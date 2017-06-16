#ifndef __PRICING_METHOD_INTERNAL_H__
#define __PRICING_METHOD_INTERNAL_H__

#include <pricing_methods/european/european_analytic.h>
#include <pricing_methods/american/finite_difference/american_finite_difference.h>

#include <common.h>
#include "pricing_method.h"

pricing_method new_pricing_method_(price_f, delta_f, gamma_f,
  theta_f, rho_f, vega_f, impl_vol_f, delete_f, pricing_data, void*);

int pm_option_price(pricing_method, option_data, double, result);
int pm_delta(pricing_method, option_data, double, result);
int pm_gamma(pricing_method, option_data, double, result);
int pm_theta(pricing_method, option_data, double, result);
int pm_rho(pricing_method, option_data, double, result);
int pm_vega(pricing_method, option_data, double, result);
int pm_ivf(pricing_method, option_data, double, double, result);

#endif /* __PRICING_METHOD_INTERNAL_H__ */
