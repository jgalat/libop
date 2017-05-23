#include <stdlib.h>

#include "pricing_method.h"

struct pricing_method_ {
  price_function    option_price;
  pricing_data      pricing_data;
  void              *pm_data;
};

pricing_method new_pricing_method(method_id id, volatility v, risk_free_rate r,
  dividend d) {
  pricing_data pd = new_pricing_data(v, r, d);
  if (!pd) {
    return NULL;
  }
  switch (id) {
    case EU_ANALYTIC:
      return new_european_analytic(pd);
    case AM_FD:
      return new_american_finite_difference(pd);
    default:
      break;
  }
  return NULL;
}

pricing_method new_pricing_method_(price_function pf, pricing_data pd,
  void *pm_d) {
  pricing_method pm = malloc(sizeof(struct pricing_method_));
  if (pm) {
    pm->option_price = pf;
    pm->pricing_data = pd;
    pm->pm_data = pm_d;
  }
  return pm;
}

int pm_option_price(pricing_method pm, option_data od, double S, date ttl,
  result r) {
  return pm->option_price(od, pm->pricing_data, S, ttl, r, pm->pm_data);
}
