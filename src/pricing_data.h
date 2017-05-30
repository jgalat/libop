#ifndef __PRICING_DATA_H__
#define __PRICING_DATA_H__

#include <parameter.h>

struct pricing_data_;
typedef struct pricing_data_ * pricing_data;

pricing_data new_pricing_data(volatility, risk_free_rate, dividend);

volatility pd_get_volatility(pricing_data pd);
void pd_set_volatility(pricing_data pd, volatility vol);
risk_free_rate pd_get_risk_free_rate(pricing_data pd);
void pd_set_risk_free_rate(pricing_data pd, risk_free_rate r);
dividend pd_get_dividend(pricing_data pd);

#endif /* __PRICING_DATA_H__ */
