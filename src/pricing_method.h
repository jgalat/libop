#ifndef __PRICING_METHOD_H__
#define __PRICING_METHOD_H__

#include <common.h>

struct pricing_method_;
typedef struct pricing_method_ * pricing_method;

#include <pricing_methods/pricing_methods.h>

pricing_method new_pricing_method(method_id, volatility, risk_free_rate, dividend);
pricing_method new_pricing_method_(price_f, delta_f, gamma_f,
  theta_f, rho_f, vega_f, pricing_data, void*);

int pm_option_price(pricing_method, option_data, double, date, result);

#endif /* __PRICING_METHOD_H__ */
