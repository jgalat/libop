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

double result_get_price(result);
double result_get_delta(result);
double result_get_gamma(result);
double result_get_theta(result);
double result_get_rho(result);
double result_get_vega(result);
volatility result_get_impl_vol(result);

#ifdef __cplusplus
}
#endif

#endif /* __RESULT_H__ */
