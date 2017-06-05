#ifndef __PRICING_METHOD_H__
#define __PRICING_METHOD_H__

#include <common.h>
#include <pm_options.h>

struct pricing_method_;
typedef struct pricing_method_ * pricing_method;

#include <pricing_methods/pricing_methods.h>

pricing_method new_pricing_method(method_id, volatility, risk_free_rate, dividend);
pricing_method new_pricing_method_(price_f, delta_f, gamma_f,
  theta_f, rho_f, vega_f, impl_vol_f, delete_f, pricing_data, void*);

void delete_pricing_method(pricing_method);

int pm_set_options(pricing_method, pm_options);

int pm_option_price(pricing_method, option_data, double, date, result);
int pm_delta(pricing_method, option_data, double, date, result);
int pm_gamma(pricing_method, option_data, double, date, result);
int pm_theta(pricing_method, option_data, double, date, result);
int pm_rho(pricing_method, option_data, double, date, result);
int pm_vega(pricing_method, option_data, double, date, result);
int pm_ivf(pricing_method, option_data, double, date, result);

#endif /* __PRICING_METHOD_H__ */
