#ifndef __PRICING_DATA_H__
#define __PRICING_DATA_H__

#include <parameter.h>
#include <dividend.h>

struct pricing_data_;
typedef struct pricing_data_ * pricing_data;

pricing_data new_pricing_data(volatility, risk_free_rate, dividend);

void delete_pricing_data(pricing_data);

volatility pd_get_volatility(pricing_data);
int pd_set_volatility(pricing_data, volatility);
risk_free_rate pd_get_risk_free_rate(pricing_data);
int pd_set_risk_free_rate(pricing_data, risk_free_rate);
dividend pd_get_dividend(pricing_data);

#endif /* __PRICING_DATA_H__ */
