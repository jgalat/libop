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

void delete_pricing_data(pricing_data pd) {
  free(pd);
}


/* TODO errors (null checks) */
inline volatility pd_get_volatility(pricing_data pd) {
  return pd->vol;
}

inline int pd_set_volatility(pricing_data pd, volatility vol) {
  if (!pd)
    return -1;
  pd->vol = vol;
  return 0;
}

inline risk_free_rate pd_get_risk_free_rate(pricing_data pd) {
  return pd->r;
}

inline int pd_set_risk_free_rate(pricing_data pd, risk_free_rate r) {
  if (!pd)
    return -1;
  pd->r = r;
  return 0;
}

inline dividend pd_get_dividend(pricing_data pd) {
  return pd->d;
}
