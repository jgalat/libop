#ifndef __BSM_NON_UNIFORM_GRID__
#define __BSM_NON_UNIFORM_GRID__

#include "BlackScholesModel.h"

#ifdef __cplusplus

class BSMUniformGrid : public BlackScholesModel {
  public:
    BSMUniformGrid(int grid_size, BSM_OT ot, double S, double vol, double rfr,
      double strike, double cont_div, int discdiv_n,
      double *discdiv_date, double *discdiv_ammo, double period,
      double end_time, double tol, double abs_tol);
    ~BSMUniformGrid();

    double v();
    double delta();
    double gamma();
    double theta();

  protected:
    void initializeDataStructs(void *CLC_simulator);
    void settings(SD_simulationSettings);
    void definition(double *x, double *d, double *alg, double t, double *dx);
    void zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc);
    void handlerPos(int i, double *x, double *d, double *alg, double t);
    void handlerNeg(int i, double *x, double *d, double *alg, double t);
    void output(int i, double *x, double *d, double *alg, double t, double *out);
    void outputAll(int i, double *x, double *d, double *alg, double t, double *out);

  private:
    SD_Solver _solver;
    SD_CommInterval _comm_interval;
    double _ft, _dqmin, _dqrel;
    double _period;

    int _g_size;
    BSM_OT _op_type;
    double _sigma, _r, _K,
          _u0, _uN1, _ds,
          _cd,   //continuous dividend
          *_discdiv_date, //discrete dividend date
          *_discdiv_ammo; //discrete dividend ammount
    int   _discdiv_n, //number of discrete dividends
          _discdiv_i; //discrete dividend index

    double *_solution;
    double _v, _delta, _gamma, _theta;

    /* binds for the engine */
    MOD_settings bsms;
    InitializeDataStructs bsmids;
    SD_eq bsmo;
    CLC_eq bsmf;
    CLC_zc bsmzc;
    CLC_hnd bsmhp, bsmhn;

};

#endif /* __cplusplus */

#endif /* __BSM_NON_UNIFORM_GRID__ */
