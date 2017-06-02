#ifndef __PRICING_DATA_H__
#define __PRICING_DATA_H__

#include <parameter.h>
#include <dividend.h>

struct pricing_data_;
typedef struct pricing_data_ * pricing_data;

/* NOTE here volatility could also be an option price */
pricing_data new_pricing_data(volatility, risk_free_rate, dividend);

void delete_pricing_data(pricing_data);

int pd_set_volatility(pricing_data, volatility);
int pd_set_risk_free_rate(pricing_data, risk_free_rate);
int pd_set_option_price(pricing_data pd, double op);

volatility pd_get_volatility(pricing_data);
risk_free_rate pd_get_risk_free_rate(pricing_data);
dividend pd_get_dividend(pricing_data);
double pd_get_option_price(pricing_data pd);

#endif /* __PRICING_DATA_H__ */
