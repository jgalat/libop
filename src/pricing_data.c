#include <stdlib.h>

#include "pricing_data.h"

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
