#include <stdlib.h>

// #include "parameter.h"
#include "pricing_data.h"

struct pricing_data_ {
  volatility vol;
  risk_free_rate r;
  dividend d;
};

pricing_data new_pricing_data(volatility vol, risk_free_rate r, dividend d) {
  pricing_data pd = malloc(sizeof(struct pricing_data_));
  if (pd) {
    pd->vol = vol;
    pd->r = r;
    pd->d = d;
  }
  return pd;
}

volatility pd_get_volatility(pricing_data pd) {
  return pd->vol;
}

risk_free_rate pd_get_risk_free_rate(pricing_data pd) {
  return pd->r;
}

dividend pd_get_dividend(pricing_data pd) {
  return pd->d;
}
