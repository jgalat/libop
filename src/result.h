#ifndef __RESULT_H__
#define __RESULT_H__

#include <parameter.h>

typedef struct result_ * result;
struct result_;

result new_result();
void delete_result(result);

int result_set_v(result r, double val);
int result_set_delta(result r, double val);
int result_set_gamma(result r, double val);
int result_set_theta(result r, double val);
int result_set_rho(result r, double val);
int result_set_vega(result r, double val);
int result_set_impl_vol(result r, volatility v);

double result_get_v(result r);
double result_get_delta(result r);
double result_get_gamma(result r);
double result_get_theta(result r);
double result_get_rho(result r);
double result_get_vega(result r);
volatility result_get_impl_vol(result r);

#endif /* __RESULT_H__ */
