#include <stdlib.h>

#include "data.h"

struct option_ {
  option_type option_type;
  exercise_type exercise_type;
  date maturity;
  pricing_method pm;
};

struct pricing_method_ {
  int  (*option_price) (option, pricing_method, double S, date, result);
  /* greeks */
  //int  (*option_delta) (double S, date, result);
  /* ... */
  //int  (*option_iv) (double S, date, double V, result);

  /* parameters */
  double sigma, r, d, k;
};

option new_option(option_type ot, exercise_type et, date maturity) {
  option o = malloc(sizeof(option));
  o->option_type = ot;
  o->exercise_type = et;
  o->maturity = maturity;
  o->pm = NULL;
  return o;
}

option_type option_get_ot(option o) {
  return o->option_type;
}
exercise_type option_get_et(option o) {
  return o->exercise_type;
}
date option_get_maturity(option o) {
  return o->maturity;
}

void option_set_pricing_method(option o, pricing_method pm) {
  o->pm = pm;
}

int option_price(option o, double S, date t, result r) {
  if (pm == NULL)
    return 1;
  return o->pm->option_price(o, o->pm, S, t, r);
}


pricing_method new_pricing_method(method_identifier m, double sigma, double r, double d, double k) {
  switch (m) {
    default:
      return new_european_analytic_pm(sigma, r, d, k);
  }
}

pricing_method new_pricing_method_(int (*opt_price)(option, pricing_method, double, date, result),
                                    double sigma, double r, double d, double k) {
  pricing_method pm = malloc(sizeof(pricing_method));
  pm->sigma = sigma;
  pm->r = r;
  pm->d = d;
  pm->k = k;
  pm->option_price = opt_price;
  return pm;
}

double pm_get_sigma(pricing_method pm) {
  return pm->sigma;
}
double pm_get_r(pricing_method pm) {
  return pm->r;
}
double pm_get_d(pricing_method pm) {
  return pm->d;
}
double pm_get_k(pricing_method pm) {
  return pm->k;
}

void pm_set_sigma(pricing_method pm, double val) {
  pm->sigma = val;
}
void pm_set_r(pricing_method pm, double val) {
  pm->r = val;
}
void pm_set_d(pricing_method pm, double val) {
  pm->d = val;
}
void pm_set_k(pricing_method pm, double val) {
  pm->k = val;
}
