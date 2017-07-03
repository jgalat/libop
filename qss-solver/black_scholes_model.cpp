#include <functional>

#include <qss-solver/engine/common/utils.h>
#include <qss-solver/engine/common/model.h>
#include <qss-solver/engine/common/data.h>
#include <qss-solver/engine/classic/classic_data.h>

#include <qss-solver/engine/common/engine.h>

#include "black_scholes_model.h"

static inline double _square(double a) {
  return a*a;
}

BlackScholesModel::BlackScholesModel(int grid_size, BSM_OT ot, double smax,
  double vol, double rfr, double strike, double cont_div, int discdiv_n,
  double *discdiv_date, double *discdiv_ammo, double period,
  double end_time, double tol, double abs_tol) {

  _solver = SD_DOPRI;
  /* Possible CI_Step only for DOPRI */
  _comm_interval = CI_Step;
  _ft = end_time < 0.0 ? 0.0 : end_time;

  _dqrel = fabs(tol);
  _dqmin = fabs(abs_tol);

  _period = fabs(period);

  _g_size = grid_size < 1 ? 0 : grid_size - 1;

  _op_type = ot;

  _Smax = fabs(smax);
  _sigma = vol;
  _r = rfr;
  _K = fabs(strike);
  _cd = cont_div;

  _discdiv_date = new double[discdiv_n + 1];
  _discdiv_ammo = new double[discdiv_n + 1];
  int i;
  for (i = 0; i < discdiv_n; i++) {
    _discdiv_date[i] = _ft - discdiv_date[i];
    _discdiv_ammo[i] = discdiv_ammo[i];
  }
  _discdiv_date[discdiv_n] = _ft + 1;
  _discdiv_ammo[discdiv_n] = 0.0;
  _discdiv_n = discdiv_n;
  _discdiv_i = 0;

  _u0 = 0.0;
  switch(_op_type) {
    case CALL:
      _uN1 = MAX(_Smax - _K, 0);
      break;
    case PUT:
      _uN1 = MAX(_K - _Smax, 0);
      break;
  }

  _ds = _Smax/(_g_size + 1);
  _ds2 = _square(_ds);

  _solution = new double[_g_size*4];

  using namespace std::placeholders;
  switch (_comm_interval) {
    case CI_Sampled:
      bsmo = std::bind(&BlackScholesModel::output, this, _1, _2, _3, _4, _5, _6);
      break;
    case CI_Step:
      bsmo = std::bind(&BlackScholesModel::outputAll, this, _1, _2, _3, _4, _5, _6);
      break;
    default:
      bsmo = std::bind(&BlackScholesModel::output, this, _1, _2, _3, _4, _5, _6);
      break;
  }

  bsmf = std::bind(&BlackScholesModel::definition, this, _1, _2, _3, _4, _5);
  bsmzc = std::bind(&BlackScholesModel::zeroCrossing, this, _1, _2, _3, _4, _5, _6);
  bsmhp = std::bind(&BlackScholesModel::handlerPos, this, _1, _2, _3, _4, _5);
  bsmhn = std::bind(&BlackScholesModel::handlerNeg, this, _1, _2, _3, _4, _5);

  bsms = std::bind(&BlackScholesModel::settings, this, _1);
  bsmids = std::bind(&BlackScholesModel::initializeDataStructs,this, _1);

  engine(&bsms, &bsmids);

  _v = _solution;
  _delta = &_solution[_g_size];
  _gamma = &_solution[_g_size*2];
  _theta = &_solution[_g_size*3];
}

BlackScholesModel::~BlackScholesModel() {
  delete [] _solution;
  delete [] _discdiv_date;
  delete [] _discdiv_ammo;
}

double BlackScholesModel::get_value(double *vals, int i) {
  i = MIN(MAX(i, 0), _g_size);

  if (i == 0) {
    return _u0;
  }

  return vals[i-1];
}

double BlackScholesModel::v(int i) {
  return get_value(_v, i);
}

double BlackScholesModel::delta(int i) {
  return get_value(_delta, i);
}

double BlackScholesModel::gamma(int i) {
  return get_value(_gamma, i);
}

double BlackScholesModel::theta(int i) {
  return get_value(_theta, i);
}

void BlackScholesModel::settings(SD_simulationSettings settings) {
  settings->debug = SD_DBG_NoDebug;
  settings->parallel = FALSE;
  settings->hybrid = TRUE;
  settings->method = _solver;
}

void BlackScholesModel::definition(double *x, double *d, double *alg, double t, double *dx) {
  int i = 0;

  const double sigma2 = _square(_sigma);

  alg[_g_size*2] = 0.5*sigma2*_square(d[_g_size])*(x[1]-2.0*x[0]+_u0)/_ds2
            + (_r-_cd)*d[_g_size]*0.5*(x[1]-_u0)/_ds
            - _r*x[0];
  alg[_g_size*3] = d[0] * alg[_g_size*2];
  dx[0] = alg[_g_size*3];

  alg[_g_size*3-1] =
            0.5*sigma2*_square(d[_g_size*2-1])*(_uN1-2.0*x[_g_size-1]+x[_g_size-2])/_ds2
            + (_r-_cd)*d[_g_size*2-1]*0.5*(_uN1-x[_g_size-2])/_ds
            - _r*x[_g_size-1];
  alg[_g_size*4-1] = d[_g_size-1]*alg[_g_size*3-1];
  dx[_g_size-1] = alg[_g_size*4-1];

  for(i = 1; i < _g_size-1; i++) {
    alg[i+_g_size*2] = 0.5*sigma2*_square(d[i+_g_size])*(x[i+1]-2.0*x[i]+x[i-1])/_ds2
                + (_r-_cd)*d[i+_g_size]*0.5*(x[i+1]-x[i-1])/_ds
                - _r*x[i];
    alg[i+_g_size*3] = d[i]*alg[i+_g_size*2];
    dx[i] = alg[i+_g_size*3];
  }
}

void BlackScholesModel::zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc) {
  const double sigma2 = _square(_sigma);
  switch(i) {
    case 0:
      zc[0] = t-(_discdiv_date[_discdiv_i]);
      return;
    default:
      if(i >= 1 && i <= _g_size) {
        alg[_g_size*2] = 0.5*sigma2*_square(d[_g_size])*(x[1]-2.0*x[0]+_u0)/_ds2
                  +(_r-_cd)*d[_g_size]*0.5*(x[1]-_u0)/_ds
                  - _r*x[0];

        if (i >= 2 && i <= _g_size-2) {
          alg[i+(_g_size*2-1)] = 0.5*sigma2*_square(d[i+_g_size-1])*(x[i]-2.0*x[i-1]+x[i-2])/_ds2
            + (_r-_cd)*d[i+_g_size-1]*0.5*(x[i]-x[i-2])/_ds
            - _r*x[i-1];
        }

        alg[_g_size*3-1] = 0.5*sigma2*_square(d[_g_size*2-1])*(_uN1-2.0*x[_g_size-1]+x[_g_size-2])/_ds2
                    + (_r-_cd)*d[_g_size*2-1]*0.5*(_uN1-x[_g_size-2])/_ds
                    - _r*x[_g_size-1];
        zc[0] = alg[i+(_g_size*2-1)];
      }
  }
}

void BlackScholesModel::handlerPos(int i, double *x, double *d, double *alg, double t) {
  int i2;
  switch(i) {
    case 0:
      for(i2 = 0; i2 < _g_size; i2++) {
        d[i2+_g_size] = d[i2+_g_size]+_discdiv_ammo[_discdiv_i];
      }
      _discdiv_i++;
      return;
    default:
      if(i >= 1 && i <= _g_size) {
        d[i-1] = 1.0;
      }
  }
}

void BlackScholesModel::handlerNeg(int i, double *x, double *d, double *alg, double t) {
  if(i >= 1 && i <= _g_size) {
    d[i-1] = 0.0;
  }
}

void BlackScholesModel::output(int i, double *x, double *d, double *alg,
  double t, double *out) {
  int j;

  j = i;
  if(j >=0 && j < _g_size) {
    out[0] = x[j];
  }

  j = i-_g_size;
  if(j >=0 && j < _g_size) {
    alg[0] = 0.5*(x[1]-_u0)/_ds;
    if (j >= 1 && j < _g_size-1) {
      alg[j] = 0.5*(x[j+1]-x[j-1])/_ds;
    }
    alg[_g_size-1] = 0.5*(_uN1-x[_g_size-2])/_ds;
    out[0] = alg[j];
  }

  j = i-_g_size*2;
  if(j >=0 && j < _g_size) {
    alg[_g_size] = (x[1]-2.0*x[0]+_u0)/_ds2;
    if (j >= 1 && j < _g_size-1) {
      alg[j+_g_size] = (x[j+1]-2.0*x[j]+x[j-1])/_ds2;
    }
    alg[_g_size*2-1] = (_uN1-2.0*x[_g_size-1]+x[_g_size-2])/_ds2;
    out[0] = alg[j+_g_size];
  }

  j = i-_g_size*3;
  if(j >=0 && j < _g_size) {
    alg[_g_size*3] = d[0]*alg[_g_size*2];
    if (j >= 1 && j < _g_size-1) {
      alg[j+_g_size*3] = d[j]*alg[j+_g_size*2];
    }
    alg[_g_size*4-1] = d[_g_size-1]*alg[_g_size*3-1];
    out[0] = -alg[j+_g_size*3];
  }
}

void BlackScholesModel::outputAll(int i, double *x, double *d, double *alg,
  double t, double *out) {

  int j;

  /*
    out: 0 -> _g_size-1
    x: 0 -> _g_size-1
  */

  for (j = 0; j < _g_size; j++) {
    out[j] = x[j];
  }

  /*
    out: _g_size -> _g_size*2-1
    alg: 0 -> _g_size-1
  */

  alg[0] = 0.5*(x[1]-_u0)/_ds;
  out[_g_size] = alg[0];

  for (j = 1; j < _g_size-1; j++) {
    alg[j] = 0.5*(x[j+1]-x[j-1])/_ds;
    out[_g_size+j] = alg[j];
  }

  alg[_g_size-1] = 0.5*(_uN1-x[_g_size-2])/_ds;
  alg[_g_size*2-1] = alg[_g_size-1];

  /*
    out: _g_size*2 -> _g_size*3-1
    alg: _g_size -> _g_size*2-1
  */

  alg[_g_size] = (x[1]-2.0*x[0]+_u0)/_ds2;
  out[_g_size*2] = alg[_g_size];

  for (j = 1; j < _g_size-1 ; j++) {
    alg[j+_g_size] = (x[j+1]-2.0*x[j]+x[j-1])/_ds2;
    out[_g_size*2+j] = alg[_g_size+j];
  }

  alg[_g_size*2-1] = (_uN1-2.0*x[_g_size-1]+x[_g_size-2])/_ds2;
  out[_g_size*3-1] = alg[_g_size*2-1];

  /*
    out: _g_size*3 -> _g_size*4-1
    alg: _g_size*3 -> _g_size*4-1
  */

  alg[_g_size*3] = d[0]*alg[_g_size*2];
  out[_g_size*3] = -alg[_g_size*3];

  for (j = 1; j < _g_size-1; j++) {
    alg[j+_g_size*3] = d[j]*alg[j+_g_size*2];
    out[_g_size*3+j] = -alg[_g_size*3+j];
  }

  alg[_g_size*4-1] = d[_g_size-1]*alg[_g_size*3-1];
  out[_g_size*4-1] = -alg[_g_size*4-1];
}

void BlackScholesModel::initializeDataStructs(void *simulator_) {
  CLC_simulator simulator = (CLC_simulator) simulator_;
  int i = 0;
  int i4;
  int i5;
  int *outputs = new int[_g_size*4];
  int *states = new int[_g_size];
  simulator->data = CLC_Data(_g_size,_g_size*2,_g_size+1,0,_g_size*4,_solver,_ft,_dqmin,_dqrel);
  CLC_data modelData = simulator->data;
  double S;
  // Initialize model code.
  for(i4 = 0; i4 < _g_size; i4++) {
    S = (i4+1)*_ds;
    modelData->x[i4] = MAX(_op_type == CALL ? S-_K : _K-S, 0.0);
    modelData->d[i4+_g_size] = S;
  }

  const double sigma2 = _square(_sigma);

  if(0.5*sigma2*_square(modelData->d[_g_size])*(modelData->x[1]-2.0*modelData->x[0]+_u0)/_ds2
      + (_r-_cd)*modelData->d[_g_size]*0.5*(modelData->x[1]-_u0)/_ds
      - _r*modelData->x[0] >= 0.0) {
    modelData->d[0] = 1.0;
  }

  for(i5 = 1; i5 < _g_size-1; i5++) {
    if(0.5*sigma2*_square(modelData->d[i5+_g_size])*(modelData->x[i5+1]-2.0*modelData->x[i5]+modelData->x[i5-1])/_ds2
        + (_r-_cd)*modelData->d[i5+_g_size]*0.5*(modelData->x[i5+1]-modelData->x[i5-1])/_ds
        - _r*modelData->x[i5] >= 0.0) {
      modelData->d[i5] = 1.0;
    }
  }

  if(0.5*sigma2*_square(modelData->d[_g_size*2-1])*(_uN1-2.0*modelData->x[_g_size-1]+modelData->x[_g_size-2])/_ds2
      + (_r-_cd)*modelData->d[_g_size*2-1]*0.5*(_uN1-modelData->x[_g_size-2])/_ds
      - _r*modelData->x[_g_size-1] >= 0.0) {
    modelData->d[_g_size-1] = 1.0;
  }

  for(i = 0; i < _g_size; i++) {
    modelData->event[i].direction = 0;
  }

  const char *modelname =  "bsm";

  switch(_comm_interval) {
    case CI_Sampled:
      simulator->output = SD_Output(modelname,_g_size*4,_g_size*2,_g_size,&_period,1,0,
        _comm_interval,SD_Memory,&bsmo,_solution);
      break;
    case CI_Step:
      simulator->output = SD_Output(modelname,_g_size*4,_g_size*2,_g_size,NULL,0,0,
        _comm_interval,SD_Memory,&bsmo,_solution);
      break;
    default:
      simulator->output = SD_Output(modelname,_g_size*4,_g_size*2,_g_size,&_period,1,0,
        CI_Sampled,SD_Memory,&bsmo,_solution);
      break;
  }

	SD_output modelOutput = simulator->output;

  for(i = 0; i < _g_size; i++) {
    modelOutput->nOS[i] = 1;
    modelOutput->nSO[i]++;
  }

  SD_allocOutputMatrix(modelOutput,_g_size,_g_size*2);
  cleanVector(states,0,_g_size);
  cleanVector(outputs,0,_g_size*4);

  for(i = 0; i < _g_size; i++) {
    modelOutput->SO[i][states[i]++] = i;
    modelOutput->OS[i][outputs[i]++] = i;
  }

	simulator->model = CLC_Model(&bsmf,&bsmzc,&bsmhp,&bsmhn);
  delete [] outputs;
  delete [] states;
}

/* C */

BSM new_BSM(int grid_size, BSM_OT ot, double smax,
  double vol, double rfr, double strike, double cont_div, int discdiv_n,
  double *discdiv_date, double *discdiv_ammo, double period,
  double end_time, double tol, double abs_tol) {
  return reinterpret_cast<BSM>(new BlackScholesModel(grid_size,ot,
    smax,vol,rfr,strike,cont_div,discdiv_n,discdiv_date,discdiv_ammo,
    period,end_time,tol,abs_tol));
}

void delete_BSM(BSM bsm) {
  delete reinterpret_cast<BlackScholesModel*>(bsm);
}

double BSM_v(BSM bsm, int i) {
  return reinterpret_cast<BlackScholesModel*>(bsm)->v(i);
}

double BSM_delta(BSM bsm, int i) {
  return reinterpret_cast<BlackScholesModel*>(bsm)->delta(i);
}

double BSM_gamma(BSM bsm, int i) {
  return reinterpret_cast<BlackScholesModel*>(bsm)->gamma(i);
}

double BSM_theta(BSM bsm, int i) {
  return reinterpret_cast<BlackScholesModel*>(bsm)->theta(i);
}
