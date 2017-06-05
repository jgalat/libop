#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <qss-solver/black_scholes_model.h>
#include <impl_vol_methods/impl_vol_methods.h>
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

static BSM BSM_(int grid_size, double tol, double abstol,
  option_data od, pricing_data pd) {

  option_type type = od_get_option_type(od);
  BSM_OT ot = option_type_BSM_OT(type);
  date maturity = od_get_maturity(od);

  volatility  sigma = pd_get_volatility(pd);
  risk_free_rate r = pd_get_risk_free_rate(pd);
  dividend divi = pd_get_dividend(pd);
  double K = od_get_strike(od);

  double d;
  int dd_n;
  double *dd_d, *dd_a;
  if (div_get_type(divi) == DIV_CONTINUOUS) {
    d = div_cont_get_val(divi);
    dd_n = 0;
    dd_d = NULL;
    dd_a = NULL;
  } else {
    d = 0.0;
    dd_n = div_disc_get_n(divi);
    dd_d = div_disc_get_dates(divi);
    dd_a = div_disc_get_ammounts(divi);
  }

  double smax = 5 * K;

  return new_BSM(grid_size, ot, smax, sigma, r, K, d, dd_n, dd_d, dd_a,
    maturity, tol, abstol);
}

static double calculate_bsmf(BSM_F bsmf, option_data od, pricing_data pd,
  double S, date ttl, pm_options pmo) {

  int f = 0;
  if (!pmo) {
    pmo = new_pm_options();
    f = 1;
  }

  int N = pm_options_get_N(pmo);
  double tol = pm_options_get_tol(pmo);
  double abstol = pm_options_get_abstol(pmo);

  int i;
  double K = od_get_strike(od),
         smax = 5 * K,
         ds   = smax / ((double) N);

  BSM bsm[2];

  //#pragma omp parallel for
  for (i = 0; i < 2; i++) {
    bsm[i] = BSM_(N + (N * i), tol, abstol, od, pd);
  }

  dividend d = pd_get_dividend(pd);
  if (div_get_type(d) == DIV_DISCRETE) {
    int ndivs = div_disc_get_n(d);
    double *ammounts = div_disc_get_ammounts(d);

    for (i = 0; i < ndivs; i++)
      S -= ammounts[i];
  }

  static const int np = 4;

  double y50[np], y100[np], s[np], y[np];

  int n = (int) (((double) N) / smax * S);
  int p50[4] = { n-1, n, n+1, n+2 };
  int p100[4] = { 2*n-2, 2*n, 2*n+2, 2*n+4 };

  for (i = 0; i < np; i++) {
    s[i] = ds * p50[i];
    y50[i] = bsmf(bsm[0], p50[i], ttl);
    y100[i] = bsmf(bsm[1], p100[i], ttl);
    /* richardson */
    y[i] = (4.0 * y100[i] - y50[i]) / 3.0;
  }

  delete_BSM(bsm[0]);
  delete_BSM(bsm[1]);

  if (f) {
    delete_pm_options(pmo);
  }

  return lagrange_interpolation(S, s, y, np);
}

static double iv_f(volatility vol, option_data od, pricing_data pd,
  double S, date ttl, pm_options pmo, void *pm_data) {
  double V = pd_get_option_price(pd);
  pd_set_volatility(pd, vol);
  double result = calculate_bsmf(BSM_v, od, pd, S, ttl, pmo) - V;
  pd_set_option_price(pd, V);
  return result;
}

static int impl_vol(option_data od, pricing_data pd, double S,
  date ttl_, result ret, pm_options pmo, void *pm_data) {

  impl_vol_options ivo = new_impl_vol_options(5, 1e-5, 0.25, 0.75, od, pd, S, ttl_,
    pmo, pm_data);

  int res = secant_method(iv_f, ivo, ret);

  delete_impl_vol_options(ivo);
  return res;
}


static int option_price(option_data od, pricing_data pd, double S,
  date ttl, result ret, pm_options pmo, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_v, od, pd, S, ttl, pmo);

  set_result(ret, result);
  return 0;
}

static int greek_delta(option_data od, pricing_data pd, double S,
  date ttl, result ret, pm_options pmo, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_delta, od, pd, S, ttl, pmo);

  set_result(ret, result);
  return 0;
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  date ttl, result ret, pm_options pmo, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_gamma, od, pd, S, ttl, pmo);

  set_result(ret, result);
  return 0;
}

static int greek_theta(option_data od, pricing_data pd, double S,
  date ttl, result ret, pm_options pmo, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */

  double result = calculate_bsmf(BSM_theta, od, pd, S, ttl, pmo);

  set_result(ret, result);
  return 0;
}

static int greek_rho(option_data od, pricing_data pd, double S,
  date ttl, result ret, pm_options pmo, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */
  static const double delta = 0.0001;

  risk_free_rate r = pd_get_risk_free_rate(pd);

  pd_set_risk_free_rate(pd, r - delta);
  double f1 = calculate_bsmf(BSM_v, od, pd, S, ttl, pmo);

  pd_set_risk_free_rate(pd, r + delta);
  double f2 = calculate_bsmf(BSM_v, od, pd, S, ttl, pmo);

  pd_set_risk_free_rate(pd, r);
  double result =  (f2 - f1) / (2 * delta);

  set_result(ret, result);
  return 0;
}

static int greek_vega(option_data od, pricing_data pd, double S,
  date ttl, result ret, pm_options pmo, void *pm_data) {

  // exercise_type et = option_get_et(o);
  /* check if it is ame... etcccc */
  static const double delta = 0.0001;

  volatility vol = pd_get_volatility(pd);

  pd_set_volatility(pd, vol - delta);
  double f1 = calculate_bsmf(BSM_v, od, pd, S, ttl, pmo);

  pd_set_volatility(pd, vol + delta);
  double f2 = calculate_bsmf(BSM_v, od, pd, S, ttl, pmo);

  pd_set_volatility(pd, vol);
  double result =  (f2 - f1) / (2 * delta);

  set_result(ret, result);
  return 0;
}

pricing_method new_american_finite_difference(pricing_data pd) {
  return new_pricing_method_(option_price, greek_delta, greek_gamma, greek_theta,
    greek_rho, greek_vega, impl_vol, NULL, pd, NULL);
}
