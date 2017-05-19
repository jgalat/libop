#ifndef __BLACK_SCHOLES_H__
#define __BLACK_SCHOLES_H__

#ifdef __cplusplus
#include <qss-solver/engine/common/data.h>
#include <qss-solver/engine/classic/classic_data.h>
#include <qss-solver/engine/classic/classic_simulator.h>

class BlackScholesModel {
  public:
    BlackScholesModel();
    ~BlackScholesModel();
    void testrun();

  private:
    void initializeDataStructs(void *CLC_simulator);
    void settings(SD_simulationSettings);
    void definition(double *x, double *d, double *alg, double t, double *dx);
    void zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc);
    void handlerPos(int i, double *x, double *d, double *alg, double t);
    void handlerNeg(int i, double *x, double *d, double *alg, double t);
    void output(int i, double *x, double *d, double *alg, double t, double *out);

    SD_Solver solver;
    double ft, dqmin, dqrel;

    CLC_data modelData;

    int N;
    double __PAR_Smax, __PAR_sigma, __PAR_r, __PAR_K, __PAR_d,
          __PAR_u0, __PAR_uN1, __PAR_ds, __PAR_ds2;
    //double __PAR_td[365] = { [0 ... 364] = 2.0};
    //double __PAR_di[365] = { 0 };
};
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct black_scholes_model_ *black_scholes_model;

  struct black_scholes_model_;

  black_scholes_model new_black_scholes_model();
  void black_scholes_model_testrun(black_scholes_model);

#ifdef __cplusplus
}
#endif

#endif /* __BLACK_SCHOLES_H__ */
