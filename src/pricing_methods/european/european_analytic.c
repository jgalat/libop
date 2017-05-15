#include <math.h>

#include <pricing_methods/european/european_analytic.h>

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

int european_analytic_option_price(option_data od, pricing_data pd, double S, date ttl, result ret) {
  // exercise_type et = option_get_et(o);
  /* check if it is eur... etcccc */

  option_type type = od_get_option_type(od);
  // date maturity = option_get_maturity(o); /* no use here */

  double  sigma = pd_get_volatility(pd),
          r = pd_get_risk_free_rate(pd),
          d = pd_get_dividend(pd),
          K = od_get_strike(od);

  double d1, d2, result;

  d1 = (log(S / K) + (r - d + powf(sigma, 2) / 2) * ttl) / (sigma * sqrt(ttl));
  d2 = d1 - sigma * sqrt(ttl);

  if (type == OPTION_CALL) {
    result = S * exp(-d * ttl) * cdf(d1) - K * exp(-r * ttl) * cdf(d2);
  } else {
    result = K * exp(-r * ttl) * cdf(-d2) - S * exp(-d * ttl) * cdf(-d1);
  }

  set_result(ret, result);
  return 0;
}

pricing_method new_european_analytic(pricing_data pd) {
  return new_pricing_method_(european_analytic_option_price, pd);
}
