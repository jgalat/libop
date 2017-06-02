#include <stdlib.h>

#include "pricing_data.h"

struct pricing_data_ {
  union {
    volatility vol;
    double option_price;
  };
  risk_free_rate r;
  dividend d;
};

pricing_data new_pricing_data(volatility vol, risk_free_rate r, dividend d) {
  pricing_data pd = (pricing_data) malloc(sizeof(struct pricing_data_));
  if (pd) {
    /* NOTE vol could be the option price */ 
    pd->vol = vol;
    pd->r = r;
    pd->d = d;
  }
  return pd;
}

void delete_pricing_data(pricing_data pd) {
  free(pd);
}

/* TODO errors (null checks) */
inline volatility pd_get_volatility(pricing_data pd) {
  return pd->vol;
}

int pd_set_volatility(pricing_data pd, volatility vol) {
  if (!pd)
    return -1;
  pd->vol = vol;
  return 0;
}

inline double pd_get_option_price(pricing_data pd) {
  return pd->option_price;
}

int pd_set_option_price(pricing_data pd, double op) {
  if (!pd)
    return -1;
  pd->option_price = op;
  return 0;
}

inline risk_free_rate pd_get_risk_free_rate(pricing_data pd) {
  return pd->r;
}

int pd_set_risk_free_rate(pricing_data pd, risk_free_rate r) {
  if (!pd)
    return -1;
  pd->r = r;
  return 0;
}

inline dividend pd_get_dividend(pricing_data pd) {
  return pd->d;
}
