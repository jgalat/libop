#include <stdlib.h>
#include <math.h>
#include <result_internal.h>
#include <option_data.h>
#include <option_data_internal.h>
#include <pricing_method_internal.h>
#include <impl_vol_methods/impl_vol_methods.h>
#include <debug.h>

#include "european_analytic.h"

static double cdf(double x) {
  // constants
  double a1 =  0.254829592;
  double a2 = -0.284496736;
  double a3 =  1.421413741;
  double a4 = -1.453152027;
  double a5 =  1.061405429;
  double p  =  0.3275911;

  // Save the sign of x
  int sign = 1;
  if (x < 0)
    sign = -1;
  x = fabs(x)/sqrt(2.0);

  // A&S formula 7.1.26
  double t = 1.0/(1.0 + p*x);
  double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

  return 0.5*(1.0 + sign*y);
}

static double calculate_dividend(risk_free_rate rfr, int size, double *dates,
  double *ammounts) {

  int i;
  double dividend = 0.0;
  double r = rfr_get_value(rfr);

  for (i = 0; i < size; i++)
    dividend += ammounts[i] * exp(-r * dates[i]);

  return dividend;
}

static void apply_div(risk_free_rate rfr, dividend divi, double *cd, double *S) {
  if (div_get_type(divi) == DIV_CONTINUOUS) {
    *cd = div_cont_get_val(divi);
  } else {
    *cd = 0.0;
    *S -= calculate_dividend(rfr, div_disc_get_n(divi),
            div_disc_get_dates(divi), div_disc_get_ammounts(divi));
  }
}

static double option_price_(option_data od, pricing_data pd, double S) {

  option_type type = od->opt_type;
  time_period tp = od->maturity;
  date ttl = tp_get_date(tp);

  double sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

  double d;
  apply_div(pd->r, divi, &d, &S);

  double d1, d2, result = 0;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));
  d2 = d1 - sigma * sqrt(ttl);

  switch (type) {
    case OPTION_CALL:
      result = S * exp(-d * ttl) * cdf(d1) - K * exp(-r * ttl) * cdf(d2);
      break;
    case OPTION_PUT:
      result = K * exp(-r * ttl) * cdf(-d2) - S * exp(-d * ttl) * cdf(-d1);
      break;
  }

  return result;
}

static double iv_f(double vol, impl_vol_mf_args ivmfa) {
  pricing_data pd = ivmfa->pd;
  vol_set_value(pd->vol, vol);
  return option_price_(ivmfa->od, pd, ivmfa->S) - ivmfa->V;
}

static const char *__OPT_NOT_EU = "Option isn't European";

static int impl_vol(option_data od, pricing_data pd, double V, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  pricing_data pd0 = new_pricing_data_(pd);

  impl_vol_mf_args ivmfa = new_impl_vol_mf_args(od, pd0, V, S, pms, pm_data);

  int res = secant_method(iv_f, ivmfa, ret);

  delete_pricing_data_(pd0);
  delete_impl_vol_mf_args(ivmfa);

  return res;
}

static int option_price(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  double result = option_price_(od, pd, S);
  return result_set_price(ret, result);
}

static int price_precision(option_data od, pricing_data pd, double V, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  /* it's analytic */
  return result_set_price_precision(ret, 0.0);
}

static int option_prices(option_data od, pricing_data pd, int size, double *Ss,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  if (!Ss)
    return 0;

  double *values = (double *) malloc(sizeof(double) * size);

  int i;
  for (i = 0; i < size; i++)
    values[i] = option_price_(od, pd, Ss[i]);

  return result_set_prices(ret, values);
}

static int greek_delta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  option_type type = od->opt_type;
  time_period tp = od->maturity;
  date ttl = tp_get_date(tp);

  double sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

  double d;
  apply_div(pd->r, divi, &d, &S);

  double d1, result = 0;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  switch (type) {
    case OPTION_CALL:
      result = exp(-d * ttl) * cdf(d1);
      break;
    case OPTION_PUT:
      result = exp(-d * ttl) * (cdf(d1) - 1);
      break;
  }

  return result_set_delta(ret, result);
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  time_period tp = od->maturity;
  date ttl = tp_get_date(tp);

  double sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

  double d;
  apply_div(pd->r, divi, &d, &S);

  double d1, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  result = (exp(-d * ttl) / (S * sigma * sqrt(ttl))) * snpd;

  return result_set_gamma(ret, result);
}

static int greek_theta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  option_type type = od->opt_type;
  time_period tp = od->maturity;
  date ttl = tp_get_date(tp);

  double sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

  double d;
  apply_div(pd->r, divi, &d, &S);

  double d1, d2, snpd, result = 0;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));
  d2 = d1 - sigma * sqrt(ttl);

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  switch (type) {
    case OPTION_CALL:
      result = (-( ((S * sigma * exp(-d * ttl)) / (2 * sqrt(ttl))) * snpd )
          - r * K * exp(-r * ttl) * cdf(d2) + d * S * exp(-d * ttl) * cdf(d1));
      break;
    case OPTION_PUT:
      result = (-( ((S * sigma * exp(-d * ttl)) / (2 * sqrt(ttl))) * snpd )
          + r * K * exp(-r * ttl) * cdf(-d2) - d * S * exp(-d * ttl) * cdf(-d1));
      break;
  }

  return result_set_theta(ret, result);
}

static int greek_rho(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  option_type type = od->opt_type;
  time_period tp = od->maturity;
  date ttl = tp_get_date(tp);

  double sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

  double d;
  apply_div(pd->r, divi, &d, &S);

  double d1, d2, result = 0;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));
  d2 = d1 - sigma * sqrt(ttl);

  switch (type) {
    case OPTION_CALL:
      result =  K * ttl * exp(-r * ttl) * cdf(d2);
      break;
    case OPTION_PUT:
      result = - K * ttl * exp(-r * ttl) * cdf(-d2);
      break;
  }

  return result_set_rho(ret, result);
}

static int greek_vega(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != EU_EXERCISE) {
    __DEBUG(__OPT_NOT_EU);
    return -1;
  }

  time_period tp = od->maturity;
  date ttl = tp_get_date(tp);

  double sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

  double d;
  apply_div(pd->r, divi, &d, &S);

  double d1, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  result = S * exp(-d * ttl) * sqrt(ttl) * snpd;

  return result_set_vega(ret, result);
}

pricing_method new_european_analytic(pricing_data pd) {
  return new_pricing_method_(option_price, price_precision, option_prices, greek_delta,
    greek_gamma, greek_theta, greek_rho, greek_vega, impl_vol, NULL, pd, NULL);
}
