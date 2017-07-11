#include <stdlib.h>

#include "pricing_data.h"

pricing_data new_pricing_data(volatility vol, risk_free_rate r, dividend d) {
  if (!d)
    return NULL;
  pricing_data pd = (pricing_data) malloc(sizeof(struct pricing_data_));
  if (pd) {
    pd->vol = vol;
    pd->r = r;
    pd->d = d;
  }
  return pd;
}

pricing_data new_pricing_data_(pricing_data pd) {
  if (!pd)
    return NULL;
  return new_pricing_data(pd->vol, pd->r, pd->d);
}

void delete_pricing_data(pricing_data pd) {
  free(pd);
}
