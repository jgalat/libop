#ifndef __RESULT_H__
#define __RESULT_H__

#include <parameter.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct result_ * result;
struct result_;

result new_result();
void delete_result(result);

double result_get_v(result r);
double result_get_delta(result r);
double result_get_gamma(result r);
double result_get_theta(result r);
double result_get_rho(result r);
double result_get_vega(result r);
volatility result_get_impl_vol(result r);

#ifdef __cplusplus
}
#endif

#endif /* __RESULT_H__ */
