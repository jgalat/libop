#include <stdlib.h>

#include "pricing_data.h"
#include <debug.h>

enum {
  VOLATILITY_FLAG = 1,
  DIVIDEND_FLAG = 2
};

pricing_data new_pricing_data(volatility vol, risk_free_rate r, dividend d) {
  if (!r) {
    __DEBUG("Risk free rate is NULL");
    return NULL;
  }
  pricing_data pd = (pricing_data) malloc(sizeof(struct pricing_data_));
  if (pd) {
    pd->flag = 0;
    if (!vol) {
      vol = new_volatility(0.0);
      pd->flag |= VOLATILITY_FLAG;
    }
    pd->vol = vol;
    pd->r = r;
    if (!d) {
      d = new_continuous_dividend(0.0);
      pd->flag |= DIVIDEND_FLAG;
    }
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

int pd_null_volatility(pricing_data pd) {
    return pd->flag & VOLATILITY_FLAG;
}

void delete_pricing_data(pricing_data pd) {
  if (pd->flag & VOLATILITY_FLAG) {
    delete_volatility(pd->vol);
  }
  if (pd->flag & DIVIDEND_FLAG) {
    delete_dividend(pd->d);
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
