#include <stdlib.h>

#include "common.h"
#include "option.h"

struct option_ {
  option_data     option_data;
  pricing_method  pm;
};

option new_option(option_type ot, exercise_type et, date m, double k) {
  option o = (option) malloc(sizeof(struct option_));
  if (o) {
    o->option_data = new_option_data(ot, et, m, k);
    if (!o->option_data) {
      delete_option(o);
      return NULL;
    }
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

int option_price(option o, double S, date ttl, result r) {
  if (o)
    return pm_option_price(o->pm, o->option_data, S, ttl, r);
  return -1;
}

int option_delta(option o, double S, date ttl, result r) {
  if (o)
    return pm_delta(o->pm, o->option_data, S, ttl, r);
  return -1;
}

int option_gamma(option o, double S, date ttl, result r) {
  if (o)
    return pm_gamma(o->pm, o->option_data, S, ttl, r);
  return -1;
}

int option_theta(option o, double S, date ttl, result r) {
  if (o)
    return pm_theta(o->pm, o->option_data, S, ttl, r);
  return -1;
}

int option_rho(option o, double S, date ttl, result r) {
  if (o)
    return pm_rho(o->pm, o->option_data, S, ttl, r);
  return -1;
}

int option_vega(option o, double S, date ttl, result r) {
  if (o)
    return pm_vega(o->pm, o->option_data, S, ttl, r);
  return -1;
}
