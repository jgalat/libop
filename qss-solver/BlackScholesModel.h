#ifndef __BLACK_SCHOLES_MODEL_H__
#define __BLACK_SCHOLES_MODEL_H__

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
    virtual ~BlackScholesModel() {};
  protected:
    virtual void initializeDataStructs(void *CLC_simulator) = 0;
    virtual void settings(SD_simulationSettings) = 0;
    virtual void definition(double *x, double *d, double *alg, double t, double *dx) = 0;
    virtual void zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc) = 0;
    virtual void handlerPos(int i, double *x, double *d, double *alg, double t) = 0;
    virtual void handlerNeg(int i, double *x, double *d, double *alg, double t) = 0;
    virtual void output(int i, double *x, double *d, double *alg, double t, double *out) = 0;
    virtual void outputAll(int i, double *x, double *d, double *alg, double t, double *out) = 0;
};

#endif /* __cplusplus */

#endif /* __BLACK_SCHOLES_MODEL_H__ */
