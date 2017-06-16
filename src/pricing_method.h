#ifndef __PRICING_METHOD_H__
#define __PRICING_METHOD_H__

#include <parameter.h>
#include <dividend.h>
#include <pm_settings.h>

struct pricing_method_;
typedef struct pricing_method_ * pricing_method;

#include <pricing_methods/pricing_methods.h>

pricing_method new_pricing_method(method_id, volatility, risk_free_rate, dividend);

void delete_pricing_method(pricing_method);

int pm_set_settings(pricing_method, pm_settings);

#endif /* __PRICING_METHOD_H__ */
