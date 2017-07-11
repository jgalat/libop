#include <stdlib.h>

#include <common.h>
#include <option_data_internal.h>
#include <pricing_method_internal.h>
#include "option.h"

struct option_ {
  option_data     option_data;
  pricing_method  pm;
};

option new_option(option_type ot, exercise_type et, time_period m, double k) {
  option_data od = new_option_data(ot, et, m, k);
  if (!od)
    return NULL;
  option o = (option) malloc(sizeof(struct option_));
  if (o) {
    o->option_data = od;
    o->pm = NULL;
  }
  return o;
}

void delete_option(option o) {
  if (o) {
    delete_option_data(o->option_data);
  }
  free(o);
}

int option_set_pricing_method(option o, pricing_method pm) {
  if (!o)
    return -1;
  o->pm = pm;
  return 0;
}

int option_price(option o, double S, result r) {
  if (o)
    return pm_option_price(o->pm, o->option_data, S, r);
  return -1;
}

int option_delta(option o, double S, result r) {
  if (o)
    return pm_delta(o->pm, o->option_data, S, r);
  return -1;
}

int option_gamma(option o, double S, result r) {
  if (o)
    return pm_gamma(o->pm, o->option_data, S, r);
  return -1;
}

int option_theta(option o, double S, result r) {
  if (o)
    return pm_theta(o->pm, o->option_data, S, r);
  return -1;
}

int option_rho(option o, double S, result r) {
  if (o)
    return pm_rho(o->pm, o->option_data, S, r);
  return -1;
}

int option_vega(option o, double S, result r) {
  if (o)
    return pm_vega(o->pm, o->option_data, S, r);
  return -1;
}

int option_impl_vol(option o, double V, double S, result r) {
  if (o)
    return pm_ivf(o->pm, o->option_data, V, S, r);
  return -1;
}

int option_price_and_greeks(option o, double S, result r) {
  typedef int (*opt_f) (option, double, result);
  static const opt_f fs[6] = { option_price, option_delta,
    option_gamma, option_theta, option_rho, option_vega };

  int i, res = 0;
  for (i = 0; i < 6; i++) {
    if ((res = fs[i](o, S, r))) {
        return res;
    }
  }

  return 0;
}
