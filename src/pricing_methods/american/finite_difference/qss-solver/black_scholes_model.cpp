#include <iostream>
#include <functional>

#include <qss-solver/engine/common/utils.h>
#include <qss-solver/engine/common/model.h>
#include <qss-solver/engine/common/data.h>
#include <qss-solver/engine/classic/classic_data.h>

#include <qss-solver/engine/common/engine.h>

#include "black_scholes_model.h"

BlackScholesModel::BlackScholesModel(int grid_size, BSM_OT ot, double smax,
  double vol, double rfr, double strike, double cont_div, int discdiv_n,
  double *discdiv_date, double *discdiv_ammo,
  double end_time, double tol, double abs_tol) {

  solver = SD_DOPRI;
  ft = end_time < 0 ? 0 : end_time;

  dqrel = tol < 0 ? 0 : tol; //1e-9
  dqmin = abs_tol < 0 ? 0 : abs_tol; //1e-12

  N = grid_size < 1 ? 0 : grid_size - 1;

  op_type = ot;

  _Smax = smax;
  _sigma = vol;
  _r = rfr;
  _K = strike;
  _cd = cont_div;

  _discdiv_date = new double[discdiv_n + 1];
  _discdiv_ammo = new double[discdiv_n + 1];
  int i;
  for (i = 0; i < discdiv_n; i++) {
    _discdiv_date[i] = ft - discdiv_date[i];
    _discdiv_ammo[i] = discdiv_ammo[i]; //<-- !!!!
  }
  _discdiv_date[discdiv_n] = ft + 1;
  _discdiv_ammo[discdiv_n] = 0.0;
  _discdiv_n = discdiv_n;
  _discdiv_i = 0;

  _u0 = 0.0;
  switch(op_type) {
    case CALL:
      _uN1 = MAX(_Smax - _K, 0);
      break;
    case PUT:
      _uN1 = MAX(_K - _Smax, 0);
      break;
  }

  _ds = _Smax/(N + 1);
  _ds2 = pow(_ds,2.0);

  using namespace std::placeholders;
  MOD_settings bsms = std::bind(&BlackScholesModel::settings, this, _1);
  InitializeDataStructs bsmids = std::bind(&BlackScholesModel::initializeDataStructs,
                                            this, _1);
  engine(bsms, bsmids);

  v = _solution;
  delta = &_solution[N];
  gamma = &_solution[N*2];
  m_theta = &_solution[N*3];
}

BlackScholesModel::~BlackScholesModel() {
  int i;
  for (i = 0; i < N*4; i++) {
    free (_solution[i]);
  }
  free(_solution);
  delete _discdiv_date;
  delete _discdiv_ammo;
}

void BlackScholesModel::settings(SD_simulationSettings settings) {
  settings->debug = SD_DBG_NoDebug;
  settings->parallel = FALSE;
  settings->hybrid = TRUE;
  settings->method = solver;
}

void BlackScholesModel::definition(double *x, double *d, double *alg, double t, double *dx) {
  int i = 0;
  //modelData->scalarEvaluations++;

  alg[N*2] = 0.5*pow(_sigma,2.0)*pow(d[N],2.0)*(x[1]-2.0*x[0]+_u0)/_ds2
            + (_r-_cd)*d[N]*0.5*(x[1]-_u0)/_ds
            - _r*x[0];
  alg[N*3] = d[0] * alg[N*2];
  dx[0] = alg[N*3];

  alg[N*3-1] =
            0.5*pow(_sigma,2.0)*pow(d[N*2],2.0)*(_uN1-2.0*x[N-1]+x[N-2])/_ds2
            + (_r-_cd)*d[N*2-1]*0.5*(_uN1-x[N-2])/_ds
            - _r*x[N-1];
  alg[N*4-1] = d[N-1]*alg[N*3-1];
  dx[N-1] = alg[N*4-1];

  for(i = 1; i < N-1; i++) {
    alg[i+N*2] = 0.5*pow(_sigma,2.0)*pow(d[i+N],2.0)*(x[i+1]-2.0*x[i]+x[i-1])/_ds2
                + (_r-_cd)*d[i+N]*0.5*(x[i+1]-x[i-1])/_ds
                - _r*x[i];
    alg[i+N*3] = d[i]*alg[i+N*2];
    dx[i] = alg[i+N*3];
  }
}

void BlackScholesModel::zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc) {
  int j1 = 0;
  //modelData->zeroCrossings++;
  switch(i) {
    case 0:
      zc[0] = t-(_discdiv_date[_discdiv_i]); // <--- rev.
      return;
    default:
      if(i >= 1 && i <= N) {
        alg[N*2] = 0.5*pow(_sigma,2.0)*pow(d[N],2.0)*(x[1]-2.0*x[0]+_u0)/_ds2
                  +(_r-_cd)*d[N]*0.5*(x[1]-_u0)/_ds
                  - _r*x[0];
        j1 = i;

        if (j1 >= 1 && j1 <= N-2) {
          alg[i+(N*2-1)] = 0.5*pow(_sigma,2.0)*pow(d[j1+N-1],2.0)*(x[j1]-2.0*x[j1-1]+x[j1-2])/_ds2
                      + (_r-_cd)*d[j1+N-1]*0.5*(x[j1]-x[j1-2])/_ds
                      - _r*x[j1-1];
        }

        alg[N*3-1] = 0.5*pow(_sigma,2.0)*pow(d[N*2-1],2.0)*(_uN1-2.0*x[N-1]+x[N-2])/_ds2
                    + (_r-_cd)*d[N*2-1]*0.5*(_uN1-x[N-2])/_ds
                    - _r*x[N-1];
        zc[0] = alg[i+(N*2-1)];
      }
  }
}

void BlackScholesModel::handlerPos(int i, double *x, double *d, double *alg, double t) {
  int i2;
  switch(i) {
    case 0:
      for(i2 = 0; i2 < N; i2++) {
        d[i2+N] = d[i2+N]+_discdiv_ammo[_discdiv_i];
      }
      _discdiv_i++;
      return;
    default:
      if(i >= 1 && i <= N) {
        d[i-1] = 1.0;
      }
  }
}

void BlackScholesModel::handlerNeg(int i, double *x, double *d, double *alg, double t) {
  if(i >= 1 && i <= N) {
    d[i-1] = 0.0;
  }
}

void BlackScholesModel::output(int i, double *x, double *d, double *alg, double t, double *out) {
  int j = 0;
  int j1 = 0;
  int j2 = 0;
  int j3 = 0;
  j = i;

  if(j >=0 && j < N) {
    out[0] = x[j];
  }

  j = i-N;
  if(j >=0 && j < N) {
    alg[0] = 0.5*(x[1]-_u0)/_ds;
    j1 = j;
    if (j1 >= 1 && j1 < N-1) {
      alg[j] = 0.5*(x[j1+1]-x[j1-1])/_ds;
    }
    alg[N-1] = 0.5*(_uN1-x[N-2])/_ds;
    out[0] = alg[j];
  }
  j = i-N*2;
  if(j >=0 && j < N) {
    alg[N] = (x[1]-2.0*x[0]+_u0)/_ds2;
    j2 = j;
    if (j2 >= 1 && j2 < N-1) {
      alg[j+N] = (x[j2+1]-2.0*x[j2]+x[j2-1])/_ds2;
    }
    alg[N*2-1] = (_uN1-2.0*x[N-1]+x[N-2])/_ds2;
    out[0] = alg[j+N];
  }

  j = i-N*3;
  if(j >=0 && j < N) {
    alg[N*2] = 0.5*pow(_sigma,2.0)*pow(d[N],2.0)*(x[1]-2.0*x[0]+_u0)/_ds2
              + (_r-_cd)*d[N]*0.5*(x[1]-_u0)/_ds
              - _r*x[0];
    alg[N*3-1] = 0.5*pow(_sigma,2.0)*pow(d[N*2-1],2.0)*(_uN1-2.0*x[N-1]+x[N-2])/_ds2
                + (_r-_cd)*d[N*2-1]*0.5*(_uN1-x[N-2])/_ds
                - _r*x[N-1];
    alg[N*3] = d[0]*alg[N*2];
    j3 = j;
    if (j3 >= 1 && j3 < N-1) {
      alg[j+N*3] = d[j3]*alg[j3+N*2];
    }
    alg[N*4-1] = d[N-1]*alg[N*3-1];
    out[0] = alg[j+N*3];
  }
}

void BlackScholesModel::initializeDataStructs(void *simulator_) {
  CLC_simulator simulator = (CLC_simulator) simulator_;
  int i = 0;
  int i4;
  int i5;
  int *outputs = new int[N*4];
  int *states = new int[N];
  simulator->data = CLC_Data(N,N*2,N+1,0,N*4,solver,ft,dqmin,dqrel);
  CLC_data modelData = simulator->data;
  double S;
  // Initialize model code.
  for(i4 = 0; i4 < N; i4++) {
    S = (i4+1)*_ds;
    modelData->x[i4] = MAX(op_type == CALL ? S-_K : _K-S, 0.0);
    modelData->d[i4+N] = S;
  }

  if(0.5*pow(_sigma,2.0)*pow(modelData->d[N],2.0)*(modelData->x[1]-2.0*modelData->x[0]+_u0)/_ds2
      + (_r-_cd)*modelData->d[N]*0.5*(modelData->x[1]-_u0)/_ds
      - _r*modelData->x[0] >= 0.0) {
    modelData->d[0] = 1.0;
  }

  for(i5 = 1; i5 < N-1; i5++) {
    if(0.5*pow(_sigma,2.0)*pow(modelData->d[i5+N],2.0)*(modelData->x[i5+1]-2.0*modelData->x[i5]+modelData->x[i5-1])/_ds2
        + (_r-_cd)*modelData->d[i5+N]*0.5*(modelData->x[i5+1]-modelData->x[i5-1])/_ds
        - _r*modelData->x[i5] >= 0.0) {
      modelData->d[i5] = 1.0;
    }
  }

  if(0.5*pow(_sigma,2.0)*pow(modelData->d[N*2-1],2.0)*(_uN1-2.0*modelData->x[N-1]+modelData->x[N-2])/_ds2
      + (_r-_cd)*modelData->d[N*2-1]*0.5*(_uN1-modelData->x[N-2])/_ds
      - _r*modelData->x[N-1] >= 0.0) {
    modelData->d[N-1] = 1.0;
  }

  for(i = 0; i < N; i++) {
    modelData->event[i].direction = 0;
  }

  double period[1];
  period[0] = 0.00273972602;

  using namespace std::placeholders;
  SD_eq bsmo = std::bind(&BlackScholesModel::output, this, _1, _2, _3, _4, _5, _6);

  simulator->output = SD_Output("bsm",N*4,N*2,N,period,1,0,CI_Sampled,
    SD_Memory,bsmo,&_solution);
	SD_output modelOutput = simulator->output;

  for(i = 0; i < N; i++) {
    modelOutput->nOS[i] = 1;
    modelOutput->nSO[i]++;
  }

  SD_allocOutputMatrix(modelOutput,N,N*2);
  cleanVector(states,0,N);

  cleanVector(outputs,0,N*4);

  // for(i = 0; i < N; i++) {
  //   sprintf(modelOutput->variable[i].name,"u[%d]",i+1);
  //   sprintf(modelOutput->variable[i+N].name,"delta[%d]",i+1);
  //   sprintf(modelOutput->variable[i+N*2].name,"gamma[%d]",i+1);
  //   sprintf(modelOutput->variable[i+N*3].name,"theta[%d]",i+1);
  // }

  cleanVector(outputs,0,N*4);

  for(i = 0; i < N; i++) {
    modelOutput->SO[i][states[i]++] = i;
    modelOutput->OS[i][outputs[i]++] = i;
  }

  CLC_eq bsmf = std::bind(&BlackScholesModel::definition, this, _1, _2, _3, _4, _5);
  CLC_zc bsmzc = std::bind(&BlackScholesModel::zeroCrossing, this, _1, _2, _3, _4, _5, _6);
  CLC_hnd bsmhp = std::bind(&BlackScholesModel::handlerPos, this, _1, _2, _3, _4, _5),
          bsmhn = std::bind(&BlackScholesModel::handlerNeg, this, _1, _2, _3, _4, _5);

	simulator->model = CLC_Model(bsmf,bsmzc,bsmhp,bsmhn);
  delete outputs;
  delete states;
}

/* C */

black_scholes_model new_black_scholes_model(int grid_size, BSM_OT ot, double smax,
  double vol, double rfr, double strike, double cont_div, int discdiv_n,
  double *discdiv_date, double *discdiv_ammo,
  double end_time, double tol, double abs_tol) {
  return reinterpret_cast<black_scholes_model>(new BlackScholesModel(grid_size,ot,
    smax,vol,rfr,strike,cont_div,discdiv_n,discdiv_date,discdiv_ammo,end_time,
    tol,abs_tol));
}

void delete_black_scholes_model(black_scholes_model bsm) {
  delete reinterpret_cast<BlackScholesModel*>(bsm);
}

// void black_scholes_model_testrun (black_scholes_model m) {
//   reinterpret_cast<BlackScholesModel*>(m)->testrun();
// }
