#include <stdlib.h>

// #include "parameter.h"
#include "pricing_data.h"

struct pricing_data_ {
  volatility vol;
  risk_free_rate r;
  dividend d;
};

pricing_data new_pricing_data(volatility vol, risk_free_rate r, dividend d) {
  pricing_data pd = (pricing_data) malloc(sizeof(struct pricing_data_));
  if (pd) {
    pd->vol = vol;
    pd->r = r;
    pd->d = d;
  }
  return pd;
}


/* TODO errors (null checks) */
inline volatility pd_get_volatility(pricing_data pd) {
  return pd->vol;
}

inline void pd_set_volatility(pricing_data pd, volatility vol) {
  pd->vol = vol;
}

inline risk_free_rate pd_get_risk_free_rate(pricing_data pd) {
  return pd->r;
}

inline void pd_set_risk_free_rate(pricing_data pd, risk_free_rate r) {
  pd->r = r;
}

inline dividend pd_get_dividend(pricing_data pd) {
  return pd->d;
}
