#include <string.h>
#include <stdlib.h>
#include <result_internal.h>
#include <option_data.h>
#include <option_data_internal.h>
#include <pricing_method_internal.h>
#include <qss-solver/BSMNonUniformGrid.h>
#include <impl_vol_methods/impl_vol_methods.h>
#include "american_finite_difference_non_uniform_grid.h"

static BSM_OT option_type_BSM_OT(option_type ot) {
  switch (ot) {
    case OPTION_CALL:
      return CALL;
    case OPTION_PUT:
      return PUT;
  }
  return CALL;
}

static BSM_NUG BSM_NUG_(int grid_size, double S, double tol, double abstol,
  option_data od, pricing_data pd) {

  option_type type = od->opt_type;
  BSM_OT ot = option_type_BSM_OT(type);
  time_period tp = od->maturity;
  date maturity = tp_get_date(tp);
  double period = tp_get_period(tp);

  double  sigma = vol_get_value(pd->vol);
  double r = rfr_get_value(pd->r);
  dividend divi = pd->d;
  double K = od->strike;

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

  return new_BSM_NUG(grid_size, ot, S, sigma, r, K, d, dd_n, dd_d, dd_a, period,
    maturity, tol, abstol);
}

static void apply_div(dividend d, int size, double *S) {
  if (div_get_type(d) == DIV_DISCRETE) {
    int ndivs = div_disc_get_n(d);
    double *ammounts = div_disc_get_ammounts(d);

    int i, j;
    for (i = 0; i < size; i++)
      for (j = 0; j < ndivs; j++)
        S[i] -= ammounts[j];
  }
}

static void calculate_bsmf(BSM_NUG_F bsmf, option_data od, pricing_data pd,
  int size, double *Ss, pm_settings pms, double *output) {

  int N = pm_settings_get_grid_size(pms);
  double tol = pm_settings_get_tol(pms);
  double abstol = pm_settings_get_abstol(pms);
  int i;

  BSM_NUG *bsm = (BSM_NUG *) malloc(sizeof(BSM_NUG) * size);
  //#pragma omp parallel for
  for (i = 0; i < size; i++) {
    bsm[i] = BSM_NUG_(N, Ss[i], tol, abstol, od, pd);
  }

  double *Ss_ = (double *) malloc(sizeof(double) * size);
  memcpy(Ss_, Ss, sizeof(double) * size);

  apply_div(pd->d, size, Ss_);

  for(i = 0; i < size; i++)
    output[i] = bsmf(bsm[i]);

  free(Ss_);
  for (i = 0; i < size; i++) {
    delete_BSM_NUG(bsm[i]);
  }
}

static double iv_f(double vol, impl_vol_mf_args ivmfa) {
  pricing_data pd = ivmfa->pd;
  vol_set_value(pd->vol, vol);
  double price;
  calculate_bsmf(BSM_NUG_v, ivmfa->od, pd, 1, &ivmfa->S, ivmfa->pms, &price);
  return price - ivmfa->V;
}

static int impl_vol(option_data od, pricing_data pd, double V, double S,
  result ret, pm_settings pms, void *pm_data) {

  pricing_data pd0 = new_pricing_data_(pd);

  impl_vol_mf_args ivmfa = new_impl_vol_mf_args(od, pd0, V, S, pms, pm_data);

  int res = secant_method(iv_f, ivmfa, ret);

  delete_pricing_data_(pd0);
  delete_impl_vol_mf_args(ivmfa);

  return res;
}

static int option_price(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  double price;
  calculate_bsmf(BSM_NUG_v, od, pd, 1, &S, pms, &price);

  return result_set_price(ret, price);
}

static int option_prices(option_data od, pricing_data pd, int size, double *Ss,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  if (!Ss)
    return 0;

  double *prices = (double *) malloc(sizeof(double) * size);

  calculate_bsmf(BSM_NUG_v, od, pd, size, Ss, pms, prices);

  return result_set_prices(ret, prices);
}

static int greek_delta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  double delta;
  calculate_bsmf(BSM_NUG_delta, od, pd, 1, &S, pms, &delta);

  return result_set_delta(ret, delta);
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  double gamma;
  calculate_bsmf(BSM_NUG_gamma, od, pd, 1, &S, pms, &gamma);

  return result_set_gamma(ret, gamma);
}

static int greek_theta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  double theta;
  calculate_bsmf(BSM_NUG_theta, od, pd, 1, &S, pms, &theta);

  return result_set_theta(ret, theta);
}

static int greek_rho(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  static const double delta = 0.0001;

  pricing_data pd0 = new_pricing_data_(pd);

  double r = rfr_get_value(pd0->r);

  rfr_set_value(pd0->r, r - delta);
  double f1;
  calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f1);

  rfr_set_value(pd0->r, r + delta);
  double f2;
  calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f2);

  double rho =  (f2 - f1) / (2 * delta);

  delete_pricing_data_(pd0);
  return result_set_rho(ret, rho);
}

static int greek_vega(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE)
    return -1;

  static const double delta = 0.0001;

  pricing_data pd0 = new_pricing_data_(pd);

  double vol = vol_get_value(pd0->vol);

  vol_set_value(pd0->vol, vol - delta);
  double f1;
  calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f1);

  vol_set_value(pd0->vol, vol + delta);
  double f2;
  calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f2);

  double vega =  (f2 - f1) / (2 * delta);

  delete_pricing_data_(pd0);
  return result_set_vega(ret, vega);
}

pricing_method new_american_finite_difference_non_uniform_grid(pricing_data pd) {
  return new_pricing_method_(option_price, option_prices, greek_delta,
    greek_gamma, greek_theta, greek_rho, greek_vega, impl_vol, NULL, pd, NULL);
}
