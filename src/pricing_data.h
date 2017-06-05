#ifndef __PRICING_DATA_H__
#define __PRICING_DATA_H__

#include <parameter.h>
#include <dividend.h>

typedef struct pricing_data_ * pricing_data;

struct pricing_data_ {
  union {
    volatility vol;
    double option_price;
  };
  risk_free_rate r;
  dividend d;
};

/* NOTE here volatility could also be an option price */
pricing_data new_pricing_data(volatility, risk_free_rate, dividend);

void delete_pricing_data(pricing_data);

#endif /* __PRICING_DATA_H__ */
