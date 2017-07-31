#ifndef __RESULT_INTERNAL_H__
#define __RESULT_INTERNAL_H__

#include <parameter.h>
#include "result.h"

int result_set_price(result r, double val);
int result_set_price_precision(result r, double val);
int result_set_prices(result r, double *values);
int result_set_delta(result r, double val);
int result_set_gamma(result r, double val);
int result_set_theta(result r, double val);
int result_set_rho(result r, double val);
int result_set_vega(result r, double val);
int result_set_impl_vol(result r, double val);

#endif /* __RESULT_INTERNAL_H__ */
