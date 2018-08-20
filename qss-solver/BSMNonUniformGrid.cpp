#include <functional>

#include <qss-solver/engine/common/utils.h>
#include <qss-solver/engine/classic/classic_data.h>
#include <qss-solver/engine/common/engine.h>

#include "BSMNonUniformGrid.h"

static inline double _square(double a) {
  return a*a;
}

BSMNonUniformGrid::BSMNonUniformGrid(int grid_size, BSM_OT ot, double S, double vol,
  double rfr, double strike, double cont_div, int discdiv_n,
  double *discdiv_date, double *discdiv_amo, double period,
  double end_time, double tol, double abs_tol) {

  _solver = SD_DOPRI;
  /* Possible CI_Step only for DOPRI */
  _comm_interval = CI_Step;
  _ft = fabs(end_time);

  _dqrel = fabs(tol);
  _dqmin = fabs(abs_tol);

  _period = fabs(period);

  _g_size = abs(grid_size);

  _op_type = ot;

  _sigma = vol;
  _r = rfr;
  _K = fabs(strike);
  _cd = cont_div;

  _S = fabs(S);

  _discdiv_date = new double[discdiv_n + 1];
  _discdiv_amo = new double[discdiv_n + 1];
  int i;
  for (i = 0; i < discdiv_n; i++) {
    _discdiv_date[discdiv_n-1-i] = _ft - discdiv_date[i];
    _discdiv_amo[discdiv_n-1-i] = discdiv_amo[i];
  }
  _discdiv_date[discdiv_n] = 0.0;
  _discdiv_amo[discdiv_n] = 0.0;
  _discdiv_n = discdiv_n;
  _discdiv_i = 0;

  _solution = new double[4];

  using namespace std::placeholders;
  switch (_comm_interval) {
    case CI_Sampled:
      bsmo = std::bind(&BSMNonUniformGrid::output, this, _1, _2, _3, _4, _5, _6);
      break;
    case CI_Step:
      bsmo = std::bind(&BSMNonUniformGrid::outputAll, this, _1, _2, _3, _4, _5, _6);
      break;
    default:
      bsmo = std::bind(&BSMNonUniformGrid::output, this, _1, _2, _3, _4, _5, _6);
      break;
  }

  bsmf = std::bind(&BSMNonUniformGrid::definition, this, _1, _2, _3, _4, _5);
  bsmzc = std::bind(&BSMNonUniformGrid::zeroCrossing, this, _1, _2, _3, _4, _5, _6);
  bsmhp = std::bind(&BSMNonUniformGrid::handlerPos, this, _1, _2, _3, _4, _5);
  bsmhn = std::bind(&BSMNonUniformGrid::handlerNeg, this, _1, _2, _3, _4, _5);

  bsms = std::bind(&BSMNonUniformGrid::settings, this, _1);
  bsmids = std::bind(&BSMNonUniformGrid::initializeDataStructs,this, _1);

  engine(&bsms, &bsmids);

  _v = _solution[0];
  _delta = _solution[1];
  _gamma = _solution[2];
  _theta = _solution[3];

  delete [] _solution;
}

BSMNonUniformGrid::~BSMNonUniformGrid() {
  delete [] _discdiv_date;
  delete [] _discdiv_amo;
}

double BSMNonUniformGrid::v() {
  return _v;
}

double BSMNonUniformGrid::delta() {
  return _delta;
}

double BSMNonUniformGrid::gamma() {
  return _gamma;
}

double BSMNonUniformGrid::theta() {
  return _theta;
}

void BSMNonUniformGrid::settings(SD_simulationSettings settings) {
  settings->debug = SD_DBG_NoDebug;
  settings->parallel = FALSE;
  settings->hybrid = TRUE;
  settings->method = _solver;
}

void BSMNonUniformGrid::definition(double *x, double *d, double *alg,
  double t, double *dx) {
  int i;

  const double sigma2 = _square(_sigma);
  const double gls2 = _square(_gls);

	alg[_g_size*2] = 0.5*sigma2*_square(d[_g_size])*((x[1]-2.0*x[0]+_u0-_gls/2.0*(x[1]-_u0))/(gls2*_square(d[_g_size])))
        +(_r-_cd)*d[_g_size]*0.5*((x[1]-_u0)/(d[_g_size]*_gls))
        -_r*x[0];
  alg[_g_size*3] = d[0]*alg[_g_size*2];
	dx[0] = alg[_g_size*3];

	alg[_g_size*3-1] = 0.5*sigma2*_square(d[_g_size*2-1])*((_uN1-2.0*x[_g_size-1]+x[_g_size-2]-_gls/2.0*(_uN1-x[_g_size-2]))/(gls2*_square(d[_g_size*2-1])))
      +(_r-_cd)*d[_g_size*2-1]*0.5*((_uN1-x[_g_size-2])/(d[_g_size*2-1]*_gls))
      -_r*x[_g_size-1];
  alg[_g_size*4-1] = d[_g_size-1]*alg[_g_size*3-1];
	dx[_g_size-1] = alg[_g_size*4-1];

	for(i = 1; i < _g_size-1; i++) {
		alg[_g_size*2+i] = 0.5*sigma2*_square(d[_g_size+i])*((x[i+1]-2.0*x[i]+x[i-1]-_gls/2.0*(x[i+1]-x[i-1]))/(gls2*_square(d[_g_size+i])))
        +(_r-_cd)*d[_g_size+i]*0.5*((x[i+1]-x[i-1])/(d[_g_size+i]*_gls))
        -_r*x[i];
    alg[_g_size*3+i] = d[i]*alg[_g_size*2+i];
		dx[i] = d[i]*alg[_g_size*2+i];
	}

}

void BSMNonUniformGrid::zeroCrossing(int i, double *x, double *d, double *alg,
  double t, double *zc) {
  const double sigma2 = _square(_sigma);
  const double gls2 = _square(_gls);
  switch(i) {
    case 0:
      zc[0] = t-(_discdiv_date[_discdiv_i]);
      return;
    default:
      if(i >= 1 && i <= _g_size) {
        alg[_g_size*2] = 0.5*sigma2*_square(d[_g_size])*((x[1]-2.0*x[0]+_u0-_gls/2.0*(x[1]-_u0))/(gls2*_square(d[_g_size])))
            +(_r-_cd)*d[_g_size]*0.5*((x[1]-_u0)/(d[_g_size]*_gls))
            -_r*x[0];

        if (i >= 2 && i <= _g_size-2) {
          alg[_g_size*2+i-1] = 0.5*sigma2*_square(d[_g_size+i-1])*((x[i]-2.0*x[i-1]+x[i-2]-_gls/2.0*(x[i]-x[i-2]))/(gls2*_square(d[_g_size+i-1])))
              +(_r-_cd)*d[_g_size+i-1]*0.5*((x[i]-x[i-2])/(d[_g_size+i-1]*_gls))
              -_r*x[i-1];
        }

        alg[_g_size*3-1] = 0.5*sigma2*_square(d[_g_size*2-1])*((_uN1-2.0*x[_g_size-1]+x[_g_size-2]-_gls/2.0*(_uN1-x[_g_size-2]))/(gls2*_square(d[_g_size*2-1])))
            +(_r-_cd)*d[_g_size*2-1]*0.5*((_uN1-x[_g_size-2])/(d[_g_size*2-1]*_gls))
            -_r*x[_g_size-1];
        zc[0] = alg[_g_size*2+i-1];
      }
  }
}

void BSMNonUniformGrid::handlerPos(int i, double *x, double *d,
  double *alg, double t) {
  int j;
  switch(i) {
    case 0:
      for(j = 0; j < _g_size; j++) {
        d[_g_size+j] +=_discdiv_amo[_discdiv_i];
      }
      _SMin += _discdiv_amo[_discdiv_i];
      _SMax += _discdiv_amo[_discdiv_i];
      _u0 = MAX(_op_type == CALL ? _SMin-_K : _K-_SMin, 0.0);
      _uN1 = MAX(_op_type == CALL ? _SMax-_K : _K-_SMax, 0.0);
      _discdiv_i++;
      return;
    default:
      if(i >= 1 && i <= _g_size) {
        d[i-1] = 1.0;
      }
  }
}

void BSMNonUniformGrid::handlerNeg(int i, double *x, double *d,
  double *alg, double t) {
  if(i >= 1 && i <= _g_size) {
    d[i-1] = 0.0;
  }
}

void BSMNonUniformGrid::output(int i, double *x, double *d, double *alg,
  double t, double *out) {
  const double gls2 = _square(_gls);
  const int odd = _g_size % 2;
  const int mid = _g_size / 2;

  switch(i) {
		case 0:
      if (odd)
			  out[0] = x[mid];
      else
        out[0] = (x[mid] + x[mid-1]) / 2.0;
			return;
		case 1:
      if (odd) {
        alg[mid] = 0.5*(x[mid+1]-x[mid-1])/(d[_g_size+mid]*_gls);
        out[0] = alg[mid];
      } else {
        alg[mid] = 0.5*(x[mid+1]-x[mid-1])/(d[_g_size+mid]*_gls);
        alg[mid-1] = 0.5*(x[mid]-x[mid-2])/(d[_g_size+mid-1]*_gls);
        out[0] = (alg[mid] + alg[mid-1]) / 2.0;
      }
			return;
		case 2:
      if (odd) {
        alg[_g_size+mid] = (x[mid+1]-2.0*x[mid]+x[mid-1]-_gls/2.0*(x[mid+1]-x[mid-1])) / (gls2*_square(d[_g_size+mid]));
        out[0] = alg[_g_size+mid];
      } else {
        alg[_g_size+mid] = (x[mid+1]-2.0*x[mid]+x[mid-1]-_gls/2.0*(x[mid+1]-x[mid-1])) / (gls2*_square(d[_g_size+mid]));
        alg[_g_size+mid-1] = (x[mid]-2.0*x[mid-1]+x[mid-2]-_gls/2.0*(x[mid]-x[mid-2])) / (gls2*_square(d[_g_size+mid-1]));
        out[0] = (alg[_g_size+mid] + alg[_g_size+mid-1]) / 2.0;
      }
			return;
		case 3:
      if (odd) {
        alg[_g_size*3+mid] = d[mid] * alg[_g_size*2+mid];
        out[0] = -alg[_g_size*3+mid];
      } else {
        alg[_g_size*3+mid] = d[mid] * alg[_g_size*2+mid];
        alg[_g_size*3+mid-1] = d[mid-1] * alg[_g_size*2+mid-1];
        out[0] = -(alg[_g_size*3+mid] + alg[_g_size*3+mid-1]) / 2.0;
      }
			return;
	}
}

void BSMNonUniformGrid::outputAll(int i, double *x, double *d, double *alg,
  double t, double *out) {

  const double gls2 = _square(_gls);
  const int odd = _g_size % 2;
  const int mid = _g_size / 2;

  if (odd) {
    out[0] = x[mid];

    alg[mid] = 0.5*(x[mid+1]-x[mid-1])/(d[_g_size+mid]*_gls);
    out[1] = alg[mid];

    alg[_g_size+mid] = (x[mid+1]-2.0*x[mid]+x[mid-1]-_gls/2.0*(x[mid+1]-x[mid-1])) / (gls2*_square(d[_g_size+mid]));
    out[2] = alg[_g_size+mid];

    alg[_g_size*3+mid] = d[mid] * alg[_g_size*2+mid];
    out[3] = -alg[_g_size*3+mid];
  } else {
    out[0] = (x[mid] + x[mid-1]) / 2.0;

    alg[mid] = 0.5*(x[mid+1]-x[mid-1])/(d[_g_size+mid]*_gls);
    alg[mid-1] = 0.5*(x[mid]-x[mid-2])/(d[_g_size+mid-1]*_gls);
    out[1] = (alg[mid] + alg[mid-1]) / 2.0;

    alg[_g_size+mid] = (x[mid+1]-2.0*x[mid]+x[mid-1]-_gls/2.0*(x[mid+1]-x[mid-1])) / (gls2*_square(d[_g_size+mid]));
    alg[_g_size+mid-1] = (x[mid]-2.0*x[mid-1]+x[mid-2]-_gls/2.0*(x[mid]-x[mid-2])) / (gls2*_square(d[_g_size+mid-1]));
    out[2] = (alg[_g_size+mid] + alg[_g_size+mid-1]) / 2.0;

    alg[_g_size*3+mid] = d[mid] * alg[_g_size*2+mid];
    alg[_g_size*3+mid-1] = d[mid-1] * alg[_g_size*2+mid-1];
    out[3] = -(alg[_g_size*3+mid] + alg[_g_size*3+mid-1]) / 2.0;
  }
}

/*
  d[0 - 99] = w
  d[100 - 199] = S
*/

void BSMNonUniformGrid::initializeDataStructs(void *simulator_) {
  CLC_simulator simulator = (CLC_simulator) simulator_;
  int i;
  simulator->data = CLC_Data(_g_size,_g_size*2,_g_size+1,0,_g_size*4,_solver,_ft,_dqmin,_dqrel);
  CLC_data modelData = simulator->data;


  double pf = 1.0 + 0.02 / _sigma;
  double mmf = exp(4.0 * pf * _sigma);
  _SMin = _S / mmf;
  _SMax = _S * mmf;
  _gls = (log(_SMax)-log(_SMin)) / (_g_size + 1);
  double edx = exp(_gls);

  _u0 = MAX(_op_type == CALL ? _SMin-_K : _K-_SMin, 0.0);
  _uN1 = MAX(_op_type == CALL ? _SMax-_K : _K-_SMax, 0.0);


  // Initialize model code.
  double S = _SMin * edx;
  modelData->d[_g_size] = S;
  modelData->x[0] = MAX(_op_type == CALL ? S-_K : _K-S, 0.0);
  for(i = 1; i < _g_size; i++) {
    S = modelData->d[_g_size+i-1]*edx;
    modelData->d[_g_size+i] = S;
    modelData->x[i] = MAX(_op_type == CALL ? S-_K : _K-S, 0.0);
  }

  const double sigma2 = _square(_sigma);
  const double gls2 = _square(_gls);

  if(0.5*sigma2*_square(modelData->d[_g_size])*((modelData->x[1]-2.0*modelData->x[0]+_u0-_gls/2.0*(modelData->x[1]-_u0))/(gls2*_square(modelData->d[_g_size])))
      +(_r-_cd)*modelData->d[_g_size]*0.5*((modelData->x[1]-_u0)/(modelData->d[_g_size]*_gls))
      -_r*modelData->x[0] >= 0.0) {
    modelData->d[(0)] = 1.0;
  }

  for(i = 1; i < _g_size-1; i++) {
    if(0.5*sigma2*_square(modelData->d[_g_size+i])*((modelData->x[i+1]-2.0*modelData->x[i]+modelData->x[i-1]-_gls/2.0*(modelData->x[i+1]-modelData->x[i-1]))/(gls2*_square(modelData->d[_g_size+i])))
        +(_r-_cd)*modelData->d[_g_size+i]*0.5*((modelData->x[i+1]-modelData->x[i-1])/(modelData->d[_g_size+i]*_gls))
        -_r*modelData->x[i] >= 0.0) {
      modelData->d[i] = 1.0;
    }
  }

  if(0.5*sigma2*_square(modelData->d[_g_size*2-1])*((_uN1-2.0*modelData->x[_g_size-1]+modelData->x[_g_size-2]-_gls/2.0*(_uN1-modelData->x[_g_size-2]))/(gls2*_square(modelData->d[_g_size*2-1])))
      +(_r-_cd)*modelData->d[_g_size*2-1]*0.5*((_uN1-modelData->x[_g_size-2])/(modelData->d[_g_size*2-1]*_gls))
      -_r*modelData->x[_g_size-1] >= 0.0) {
    modelData->d[_g_size-1] = 1.0;
  }

  modelData->event[0].direction = 1;
  for(i = 0; i < _g_size; i++) {
    modelData->event[i+1].direction = 0;
  }

  const char *modelname =  "bsm";

  switch(_comm_interval) {
    case CI_Sampled:
      simulator->output = SD_Output(modelname,4,_g_size*2,_g_size,&_period,1,0,
        _comm_interval,SD_Memory,&bsmo,_solution);
      break;
    case CI_Step:
      simulator->output = SD_Output(modelname,4,_g_size*2,_g_size,NULL,0,0,
        _comm_interval,SD_Memory,&bsmo,_solution);
      break;
    default:
      simulator->output = SD_Output(modelname,4,_g_size*2,_g_size,&_period,1,0,
        CI_Sampled,SD_Memory,&bsmo,_solution);
      break;
  }

  simulator->model = CLC_Model(&bsmf,&bsmzc,&bsmhp,&bsmhn);
}

/* C */

BSM_NUG new_BSM_NUG(int grid_size, BSM_OT ot, double S,
  double vol, double rfr, double strike, double cont_div, int discdiv_n,
  double *discdiv_date, double *discdiv_amo, double period,
  double end_time, double tol, double abs_tol) {
  return reinterpret_cast<BSM_NUG>(new BSMNonUniformGrid(grid_size,ot,
    S,vol,rfr,strike,cont_div,discdiv_n,discdiv_date,discdiv_amo,
    period,end_time,tol,abs_tol));
}

void delete_BSM_NUG(BSM_NUG bsm) {
  delete reinterpret_cast<BSMNonUniformGrid*>(bsm);
}

double BSM_NUG_v(BSM_NUG bsm) {
  return reinterpret_cast<BSMNonUniformGrid*>(bsm)->v();
}

double BSM_NUG_delta(BSM_NUG bsm) {
  return reinterpret_cast<BSMNonUniformGrid*>(bsm)->delta();
}
double BSM_NUG_gamma(BSM_NUG bsm) {
  return reinterpret_cast<BSMNonUniformGrid*>(bsm)->gamma();
}
double BSM_NUG_theta(BSM_NUG bsm) {
  return reinterpret_cast<BSMNonUniformGrid*>(bsm)->theta();
}
