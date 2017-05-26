#include <stdlib.h>
#include <math.h>

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

static int option_price(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

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
    default:
      result = -1;
      break;
  }

  set_result(ret, result);
  return 0;
}

static int greek_delta(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

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
      result = -1;
      break;
  }

  set_result(ret, result);
  return 0;
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

  double d1, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  result = (exp(-d * ttl) / (S * sigma * sqrt(ttl))) * snpd;

  set_result(ret, result);
  return 0;
}

static int greek_theta(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

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
    default:
      result = -1;
      break;
  }

  set_result(ret, result);
  return 0;
}

static int greek_rho(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

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
    default:
      result = -1;
      break;
  }

  set_result(ret, result);
  return 0;
}

static int greek_vega(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

  double d1, snpd, result;

  d1 = (log(S / K) + (r - d + pow(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));

  snpd = (1 / sqrt(2 * M_PI)) * exp(-pow(d1, 2) / 2);

  result = S * exp(-d * ttl) * sqrt(ttl) * snpd;

  set_result(ret, result);
  return 0;
}

pricing_method new_european_analytic(pricing_data pd) {
  return new_pricing_method_(option_price, greek_delta, greek_gamma, greek_theta,
    greek_rho, greek_vega, NULL, pd, NULL);
}
