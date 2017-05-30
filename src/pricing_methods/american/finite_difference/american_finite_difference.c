#include <stdio.h>
#include <stdlib.h>
#include <qss-solver/black_scholes_model.h>
#include "american_finite_difference.h"

static double lagrange_interpolation(double X, double *x, double *y, int N) {
  double result = 0, temp;
  int i, j;

  for (i = 0; i < N; i++) {
    temp = 1;
    for (j = 0; j < N; j++) {
      if (j != i)
        temp *= (X - x[j]) / (x[i] - x[j]);
    }
    result += temp * y[i];
  }

  return result;
}

static BSM_OT option_type_BSM_OT(option_type ot) {
  switch (ot) {
    case OPTION_CALL:
      return CALL;
    case OPTION_PUT:
      return PUT;
  }
  return CALL;
}

static BSM BSM_(int grid_size, option_data od, pricing_data pd) {
  option_type type = od_get_option_type(od);
  BSM_OT ot = option_type_BSM_OT(type);
  date maturity = od_get_maturity(od);

  volatility  sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend d = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double smax = 5 * K;

  return new_BSM(grid_size, ot, smax, sigma, r, K, d, 0, NULL, NULL,
    maturity, 1e-9, 1e-12);
}

static double calculate_bsmf(BSM_F bsmf, option_data od, pricing_data pd,
  double S, date ttl) {

  double K = od_get_strike(od),
         smax = 5 * K,
         ds   = smax / 50.0;

  BSM bsm50 = BSM_(50, od, pd),
      bsm100 = BSM_(100, od, pd);

  static const int np = 4;

  double y50[np], y100[np], s[np], y[np];

  int n = (int) (50.0 / smax * S);
  int p50[4] = { n-1, n, n+1, n+2 };
  int p100[4] = { 2*n-2, 2*n, 2*n+2, 2*n+4 };

  int i;
  for (i = 0; i < np; i++) {
    s[i] = ds * p50[i];
    y50[i] = bsmf(bsm50, p50[i], ttl);
    y100[i] = bsmf(bsm100, p100[i], ttl);
    /* richardson */
    y[i] = (4.0 * y100[i] - y50[i]) / 3.0;
  }

  delete_BSM(bsm50);
  delete_BSM(bsm100);

  return lagrange_interpolation(S, s, y, np);
}

static int option_price(option_data od, pricing_data pd,
  double S, date ttl, result ret, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_v, od, pd, S, ttl);

  set_result(ret, result);
  return 0;
}

static int greek_delta(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_delta, od, pd, S, ttl);

  set_result(ret, result);
  return 0;
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_gamma, od, pd, S, ttl);

  set_result(ret, result);
  return 0;
}

static int greek_theta(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_theta, od, pd, S, ttl);

  set_result(ret, result);
  return 0;
}

static int greek_rho(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */
  static const double delta = 0.0001;

  risk_free_rate r = pd_get_risk_free_rate(pd);

  pd_set_risk_free_rate(pd, r - delta);
  double f1 = calculate_bsmf(BSM_v, od, pd, S, ttl);

  pd_set_risk_free_rate(pd, r + delta);
  double f2 = calculate_bsmf(BSM_v, od, pd, S, ttl);

  double result =  (f2 - f1) / (2 * delta);

  set_result(ret, result);
  return 0;
}

static int greek_vega(option_data od, pricing_data pd, double S,
  date ttl, result ret, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */
  static const double delta = 0.0001;

  volatility vol = pd_get_volatility(pd);

  pd_set_volatility(pd, vol - delta);
  double f1 = calculate_bsmf(BSM_v, od, pd, S, ttl);

  pd_set_volatility(pd, vol + delta);
  double f2 = calculate_bsmf(BSM_v, od, pd, S, ttl);

  double result =  (f2 - f1) / (2 * delta);

  set_result(ret, result);
  return 0;
}

pricing_method new_american_finite_difference(pricing_data pd) {
  return new_pricing_method_(option_price, greek_delta, greek_gamma, greek_theta,
    greek_rho, greek_vega, NULL, pd, NULL);
}
