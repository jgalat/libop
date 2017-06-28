#ifndef __BLACK_SCHOLES_H__
#define __BLACK_SCHOLES_H__

typedef enum {
  CALL,
  PUT
} BSM_OT;

#ifdef __cplusplus
#include <qss-solver/engine/common/model.h>
#include <qss-solver/engine/common/data.h>
#include <qss-solver/engine/classic/classic_data.h>
#include <qss-solver/engine/classic/classic_simulator.h>

class BlackScholesModel {
  public:
    BlackScholesModel(int grid_size, BSM_OT ot, double smax,
      double vol, double rfr, double strike, double cont_div, int discdiv_n,
      double *discdiv_date, double *discdiv_ammo, double period,
      double end_time, double tol, double abs_tol);
    ~BlackScholesModel();

    /* TODO errors (index, etc) */
    double v(int i);
    double delta(int i);
    double gamma(int i);
    double theta(int i);

  private:
    void initializeDataStructs(void *CLC_simulator);
    void settings(SD_simulationSettings);
    void definition(double *x, double *d, double *alg, double t, double *dx);
    void zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc);
    void handlerPos(int i, double *x, double *d, double *alg, double t);
    void handlerNeg(int i, double *x, double *d, double *alg, double t);
    void output(int i, double *x, double *d, double *alg, double t, double *out);
    void outputAll(int i, double *x, double *d, double *alg, double t, double *out);

    double get_value(double*, int);

    SD_Solver _solver;
    SD_CommInterval _comm_interval;
    double _ft, _dqmin, _dqrel;
    double _period;
    // CLC_data modelData;

    int _g_size;
    BSM_OT _op_type;
    double _Smax, _sigma, _r, _K,
          _u0, _uN1, _ds, _ds2,
          _cd,   //continuous dividend
          *_discdiv_date, //discrete dividend date
          *_discdiv_ammo; //discrete dividend ammount
    int   _discdiv_n, //number of discrete dividends
          _discdiv_i; //discrete dividend index

    double *_solution;
    double *_v, *_delta, *_gamma, *_theta;

    /* binds for the engine */
    MOD_settings bsms;
    InitializeDataStructs bsmids;
    SD_eq bsmo;
    CLC_eq bsmf;
    CLC_zc bsmzc;
    CLC_hnd bsmhp, bsmhn;

};
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct BSM_ *BSM;

  struct BSM_;

  BSM new_BSM(int grid_size, BSM_OT ot, double smax,
    double vol, double rfr, double strike, double cont_div, int discdiv_n,
    double *discdiv_date, double *discdiv_ammo, double period,
    double end_time, double tol, double abs_tol);

  void delete_BSM(BSM);

  typedef double (*BSM_F) (BSM, int);

  double BSM_v(BSM, int);
  double BSM_delta(BSM, int);
  double BSM_gamma(BSM, int);
  double BSM_theta(BSM, int);

#ifdef __cplusplus
}
#endif

#endif /* __BLACK_SCHOLES_H__ */
