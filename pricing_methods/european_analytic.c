#include <stdlib.h>
#include <math.h>

#include "european_analytic.h"

double cdf(double x) {
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

int european_analytic_option_price(option o, pricing_method pm, double S, date t, result ret) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = option_get_ot(o);
  // date maturity = option_get_maturity(o); /* no use here */

  double sigma = pm_get_sigma(pm), r = pm_get_r(pm), d = pm_get_d(pm), K = pm_get_k(pm);
  double d1, d2, result;

  d1 = (log(S / K) + (r - d + powf(sigma, 2) / 2) * t) / (sigma * sqrt(t));
  d2 = d1 - sigma * sqrt(t);

  if (type == OPTION_CALL) {
    result = S * exp(-d * t) * cdf(d1) - K * exp(-r * t) * cdf(d2);
  } else {
    result = K * exp(-r * t) * cdf(-d2) - S * exp(-d * t) * cdf(-d1);
  }

  if (ret != NULL) {
    *ret = result;
  }
  return 0;
}

pricing_method new_european_analytic_pm(double sigma, double r, double d, double k) {
  pricing_method pm = malloc(sizeof(pricing_method));
  pm_set_sigma(pm, sigma);
  pm_set_r(pm, r);
  pm_set_d(pm, d);
  pm_set_k(pm, k);
  pm_set_option_price_f(pm, european_analytic_option_price);
  return pm;
}
