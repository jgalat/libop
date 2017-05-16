#ifndef __BLACK_SCHOLES_H__
#define __BLACK_SCHOLES_H__

#include <qss-solver/engine/classic/classic_data.h>

#ifdef __cplusplus
class BlackScholesModel {
  public:
    BlackScholesModel();
    ~BlackScholesModel();
  private:
    CLC_data modelData;

    int N;
    double __PAR_Smax, __PAR_sigma, __PAR_r, __PAR_K, __PAR_d,
          __PAR_u0, __PAR_uN1, __PAR_ds, __PAR_ds2;
    //double __PAR_td[365] = { [0 ... 364] = 2.0};
    //double __PAR_di[365] = { 0 };
};
#endif /* __cplusplus */


#endif /* __BLACK_SCHOLES_H__ */
