#include <stdlib.h>

#include "pricing_data.h"
#include <debug.h>


pricing_data new_pricing_data(volatility vol, risk_free_rate r, dividend d) {
  if (!d || !r) {
    __DEBUG("Dividend or risk free rate is NULL");
    return NULL;
  }
  pricing_data pd = (pricing_data) malloc(sizeof(struct pricing_data_));
  if (pd) {
    if (!vol) {
      vol = new_volatility(0.0);
      pd->vol_flag = 1;
    } else {
      pd->vol_flag = 0;
    }
    pd->vol = vol;
    pd->r = r;
    pd->d = d;
  }
  return pd;
}

pricing_data new_pricing_data_(pricing_data pd) {
  if (!pd)
    return NULL;
  risk_free_rate rfr = new_risk_free_rate_(pd->r);
  volatility vol = new_volatility_(pd->vol);
  return new_pricing_data(vol, rfr, pd->d);
}

void delete_pricing_data(pricing_data pd) {
  if (pd->vol_flag) {
    delete_volatility(pd->vol);
  }
  free(pd);
}

void delete_pricing_data_(pricing_data pd) {
  if (pd) {
    delete_volatility(pd->vol);
    delete_risk_free_rate(pd->r);
  }
  free(pd);
}
