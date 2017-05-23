#ifndef __BLACK_SCHOLES_H__
#define __BLACK_SCHOLES_H__

typedef enum {
  CALL,
  PUT
} BSM_OT;

#ifdef __cplusplus
#include <qss-solver/engine/common/data.h>
#include <qss-solver/engine/classic/classic_data.h>
#include <qss-solver/engine/classic/classic_simulator.h>

class BlackScholesModel {
  public:
    BlackScholesModel(int grid_size, BSM_OT ot, double smax,
      double vol, double rfr, double strike, double cont_div, int discdiv_n,
      double *discdiv_date, double *discdiv_ammo,
      double end_time, double tol, double abs_tol);
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

    // CLC_data modelData;

    int N;
    BSM_OT op_type;
    double _Smax, _sigma, _r, _K,
          _u0, _uN1, _ds, _ds2,
          _cd,   //continuous dividend
          *_discdiv_date, //discrete dividend date
          *_discdiv_ammo; //discrete dividend ammount
    int   _discdiv_i; //discrete dividend index

};
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct black_scholes_model_ *black_scholes_model;

  struct black_scholes_model_;

  black_scholes_model new_black_scholes_model(int grid_size, BSM_OT ot, double smax,
    double vol, double rfr, double strike, double cont_div, int discdiv_n,
    double *discdiv_date, double *discdiv_ammo,
    double end_time, double tol, double abs_tol);

  void black_scholes_model_testrun(black_scholes_model);

#ifdef __cplusplus
}
#endif

#endif /* __BLACK_SCHOLES_H__ */
