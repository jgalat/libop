#include <stdlib.h>
#include <math.h>
#include <impl_vol_methods/impl_vol_methods.h>

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

static double calculate_dividend(risk_free_rate r, int size, double *dates,
  double *ammounts) {

  int i;
  double dividend = 0.0;

  for (i = 0; i < size; i++)
    dividend += ammounts[i] * exp(-r * dates[i]);

  return dividend;
}

static void apply_div(risk_free_rate r, dividend divi, double *d, double *S) {
  if (div_get_type(divi) == DIV_CONTINUOUS) {
    *d = div_cont_get_val(divi);
  } else {
    *d = 0.0;
    *S -= calculate_dividend(r, div_disc_get_n(divi),
            div_disc_get_dates(divi), div_disc_get_ammounts(divi));
  }
}

static double option_price_(option_data od, pricing_data pd, double S) {

  option_type type = od_get_option_type(od);
  date ttl = od_get_maturity(od);

  volatility sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  apply_div(r, divi, &d, &S);

  double d1, d2, result;

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

static double iv_f(volatility vol, option_data od, pricing_data pd,
  double S, date ttl_, pm_options pmo, void *pm_data) {
  double V = pd_get_option_price(pd);
  pd_set_volatility(pd, vol);
  double result = option_price_(od, pd, S) - V;
  pd_set_option_price(pd, V);
  return result;
}

static int impl_vol(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {

  impl_vol_options ivo = new_impl_vol_options(10, 1e-5, 0.25, 0.75, od, pd, S, ttl_,
    pmo, pm_data);

  int res = secant_method(iv_f, ivo, ret);

  delete_impl_vol_options(ivo);
  return res;
}

static int option_price(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  double result = option_price_(od, pd, S);
  set_result(ret, result);
  return 0;
}

static int greek_delta(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  date ttl = od_get_maturity(od);

  volatility sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  apply_div(r, divi, &d, &S);

  double d1, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  switch (type) {
    case OPTION_CALL:
      result = exp(-d * ttl) * cdf(d1);
      break;
    case OPTION_PUT:
      result = exp(-d * ttl) * (cdf(d1) - 1);
      break;
    default:
      return -1;
      break;
  }

  set_result(ret, result);
  return 0;
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  date ttl = od_get_maturity(od);

  volatility sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  apply_div(r, divi, &d, &S);

  double d1, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  result = (exp(-d * ttl) / (S * sigma * sqrt(ttl))) * snpd;

  set_result(ret, result);
  return 0;
}

static int greek_theta(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  date ttl = od_get_maturity(od);

  volatility sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  apply_div(r, divi, &d, &S);

  double d1, d2, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));
  d2 = d1 - sigma * sqrt(ttl);

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  double daysperyear = 365.0; /* FIXME */

  switch (type) {
    case OPTION_CALL:
      result = (1.0 / daysperyear)
        * (-( ((S * sigma * exp(-d * ttl)) / (2 * sqrt(ttl))) * snpd )
          - r * K * exp(-r * ttl) * cdf(d2) + d * S * exp(-d * ttl) * cdf(d1));
      break;
    case OPTION_PUT:
      result = (1.0 / daysperyear)
        * (-( ((S * sigma * exp(-d * ttl)) / (2 * sqrt(ttl))) * snpd )
          + r * K * exp(-r * ttl) * cdf(-d2) - d * S * exp(-d * ttl) * cdf(-d1));
      break;
  }

  set_result(ret, result);
  return 0;
}

static int greek_rho(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  date ttl = od_get_maturity(od);

  volatility sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  apply_div(r, divi, &d, &S);

  double d1, d2, result;

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

  set_result(ret, result);
  return 0;
}

static int greek_vega(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  date ttl = od_get_maturity(od);

  volatility sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  apply_div(r, divi, &d, &S);

  double d1, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  result = S * exp(-d * ttl) * sqrt(ttl) * snpd;

  set_result(ret, result);
  return 0;
}

pricing_method new_european_analytic(pricing_data pd) {
  return new_pricing_method_(option_price, greek_delta, greek_gamma, greek_theta,
    greek_rho, greek_vega, impl_vol, NULL, pd, NULL);
}
