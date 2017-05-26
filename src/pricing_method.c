#include <stdlib.h>

#include "pricing_method.h"

struct pricing_method_ {
  price_f       option_price;
  delta_f       delta;
  gamma_f       gamma;
  theta_f       theta;
  rho_f         rho;
  vega_f        vega;
  pricing_data  pricing_data;
  void          *pm_data;
  delete_f      delete;
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

pricing_method new_pricing_method_(price_f pf, delta_f df, gamma_f gf,
  theta_f tf, rho_f rf, vega_f vf, delete_f dlf, pricing_data pd, void *pm_d) {
  pricing_method pm = malloc(sizeof(struct pricing_method_));
  if (pm) {
    pm->option_price = pf;
    pm->delta = df;
    pm->gamma = gf;
    pm->theta = tf;
    pm->rho = rf;
    pm->vega = vf;
    pm->pricing_data = pd;
    pm->pm_data = pm_d;
    pm->delete = dlf;
  }
  return pm;
}

void delete_pricing_method(pricing_method pm) {
  if (pm)
    pm->delete((void*) pm);
  free(pm);
}

int pm_option_price(pricing_method pm, option_data od, double S, date ttl,
  result r) {

  if (pm->option_price)
    return pm->option_price(od, pm->pricing_data, S, ttl, r, pm->pm_data);

  return -1;
}

int pm_delta(pricing_method pm, option_data od, double S, date ttl,
  result r) {

  if (pm->delta)
    return pm->delta(od, pm->pricing_data, S, ttl, r, pm->pm_data);

  return -1;
}

int pm_gamma(pricing_method pm, option_data od, double S, date ttl,
  result r) {

  if (pm->gamma)
    return pm->gamma(od, pm->pricing_data, S, ttl, r, pm->pm_data);

  return -1;
}

int pm_theta(pricing_method pm, option_data od, double S, date ttl,
  result r) {

  if (pm->theta)
    return pm->theta(od, pm->pricing_data, S, ttl, r, pm->pm_data);

  return -1;
}

int pm_rho(pricing_method pm, option_data od, double S, date ttl,
  result r) {

  if (pm->rho)
    return pm->rho(od, pm->pricing_data, S, ttl, r, pm->pm_data);

  return -1;
}

int pm_vega(pricing_method pm, option_data od, double S, date ttl,
  result r) {

  if (pm->vega)
    return pm->vega(od, pm->pricing_data, S, ttl, r, pm->pm_data);

  return -1;
}
