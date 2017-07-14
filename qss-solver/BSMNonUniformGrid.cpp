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
  double *discdiv_date, double *discdiv_ammo, double period,
  double end_time, double tol, double abs_tol) {

  // _solver = SD_DOPRI;
  // /* Possible CI_Step only for DOPRI */
  // _comm_interval = CI_Step;
  // _ft = end_time < 0.0 ? 0.0 : end_time;
  //
  // _dqrel = fabs(tol);
  // _dqmin = fabs(abs_tol);
  //
  // _period = fabs(period);
  //
  // _g_size = abs(grid_size);
  //
  // _op_type = ot;
  //
  // _sigma = vol;
  // _r = rfr;
  // _K = fabs(strike);
  // _cd = cont_div;
  //
  // _discdiv_date = new double[discdiv_n + 1];
  // _discdiv_ammo = new double[discdiv_n + 1];
  // int i;
  // for (i = 0; i < discdiv_n; i++) {
  //   _discdiv_date[i] = _ft - discdiv_date[i];
  //   _discdiv_ammo[i] = discdiv_ammo[i];
  // }
  // _discdiv_date[discdiv_n] = _ft + 1;
  // _discdiv_ammo[discdiv_n] = 0.0;
  // _discdiv_n = discdiv_n;
  // _discdiv_i = 0;
  //
  // _u0 = 0.0;
  // switch(_op_type) {
  //   case CALL:
  //     _uN1 = MAX(_Smax - _K, 0);
  //     break;
  //   case PUT:
  //     _uN1 = MAX(_K - _Smax, 0);
  //     break;
  // }
  //
  // _ds = _Smax/(_g_size + 1);
  //
  // _solution = new double[4];
  //
  // using namespace std::placeholders;
  // switch (_comm_interval) {
  //   case CI_Sampled:
  //     bsmo = std::bind(&BSMNonUniformGrid::output, this, _1, _2, _3, _4, _5, _6);
  //     break;
  //   case CI_Step:
  //     bsmo = std::bind(&BSMNonUniformGrid::outputAll, this, _1, _2, _3, _4, _5, _6);
  //     break;
  //   default:
  //     bsmo = std::bind(&BSMNonUniformGrid::output, this, _1, _2, _3, _4, _5, _6);
  //     break;
  // }
  //
  // bsmf = std::bind(&BSMNonUniformGrid::definition, this, _1, _2, _3, _4, _5);
  // bsmzc = std::bind(&BSMNonUniformGrid::zeroCrossing, this, _1, _2, _3, _4, _5, _6);
  // bsmhp = std::bind(&BSMNonUniformGrid::handlerPos, this, _1, _2, _3, _4, _5);
  // bsmhn = std::bind(&BSMNonUniformGrid::handlerNeg, this, _1, _2, _3, _4, _5);
  //
  // bsms = std::bind(&BSMNonUniformGrid::settings, this, _1);
  // bsmids = std::bind(&BSMNonUniformGrid::initializeDataStructs,this, _1);
  //
  // engine(&bsms, &bsmids);
  //
  // _v = _solution[0];
  // _delta = _solution[1];
  // _gamma = _solution[2];
  // _theta = _solution[3];
  //
  // delete _solution;
}

BSMNonUniformGrid::~BSMNonUniformGrid() {
  delete [] _discdiv_date;
  delete [] _discdiv_ammo;
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

void BSMNonUniformGrid::settings(SD_simulationSettings) {
  settings->debug = SD_DBG_NoDebug;
  settings->parallel = FALSE;
  settings->hybrid = TRUE;
  settings->method = _solver;
}

void BSMNonUniformGrid::definition(double *x, double *d, double *alg,
  double t, double *dx) {

}

void BSMNonUniformGrid::zeroCrossing(int i, double *x, double *d, double *alg,
  double t, double *zc) {

}

void BSMNonUniformGrid::handlerPos(int i, double *x, double *d,
  double *alg, double t) {

}

void BSMNonUniformGrid::handlerNeg(int i, double *x, double *d,
  double *alg, double t) {

}

void BSMNonUniformGrid::output(int i, double *x, double *d, double *alg,
  double t, double *out) {

}

void BSMNonUniformGrid::outputAll(int i, double *x, double *d, double *alg,
  double t, double *out) {

}

void BSMNonUniformGrid::initializeDataStructs(void *CLC_simulator) {

}
