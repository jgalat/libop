#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <result_internal.h>
#include <option_data.h>
#include <option_data_internal.h>
#include <pricing_method_internal.h>
#include <qss-solver/engine/common/utils.h>
#include <qss-solver/BSMNonUniformGrid.h>
#include <impl_vol_methods/impl_vol_methods.h>
#include <debug.h>

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

static int calculate_bsmf(BSM_NUG_F bsmf, option_data od, pricing_data pd,
  int size, double *Ss, pm_settings pms, double *output) {

  if (div_get_type(pd->d) == DIV_DISCRETE) {
    __DEBUG("Discrete dividends is not supported by the American Finite Difference Non Uniform Grid pricing method.");
    return -1;
  }

  int N = pm_settings_get_grid_size(pms);
  double tol = pm_settings_get_tol(pms);
  double abstol = pm_settings_get_abstol(pms);
  int i;

  BSM_NUG *bsm = (BSM_NUG *) malloc(sizeof(BSM_NUG) * size);

  //#pragma omp parallel for
  for (i = 0; i < size; i++) {
    bsm[i] = BSM_NUG_(N, Ss[i], tol, abstol, od, pd);
  }

  for(i = 0; i < size; i++)
    output[i] = bsmf(bsm[i]);

  for (i = 0; i < size; i++) {
    delete_BSM_NUG(bsm[i]);
  }
  free(bsm);
  return 0;
}

static double iv_f(double vol, impl_vol_mf_args ivmfa) {
  pricing_data pd = ivmfa->pd;
  vol_set_value(pd->vol, vol);
  double price;
  calculate_bsmf(BSM_NUG_v, ivmfa->od, pd, 1, &ivmfa->S, ivmfa->pms, &price);
  return price - ivmfa->V;
}

static const char *__OPT_NOT_AM = "Option isn't American";

static int impl_vol(option_data od, pricing_data pd, double V, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
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

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double price;
  int err = calculate_bsmf(BSM_NUG_v, od, pd, 1, &S, pms, &price);

  if (err)
    return err;

  return result_set_price(ret, price);
}

static int price_precision(option_data od, pricing_data pd, double V, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  int f;
  if (!pms) {
    pms = new_pm_settings();
    f = 1;
  }

  static const int MAX_REASONABLE_GRID = 500;

  int curr_grid_size = pm_settings_get_grid_size(pms);
  int bigger_grid_size = MIN(curr_grid_size * 3, MAX_REASONABLE_GRID);
  pm_settings_set_grid_size(pms, bigger_grid_size);

  double price;
  int err = calculate_bsmf(BSM_NUG_v, od, pd, 1, &S, pms, &price);
  pm_settings_set_grid_size(pms, curr_grid_size);

  if (f) {
    delete_pm_settings(pms);
  }

  if (err)
    return err;

  double precision = fabs(V - price);

  /* replace with better price? */

  return result_set_price_precision(ret, precision);
}

static int option_prices(option_data od, pricing_data pd, int size, double *Ss,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  if (!Ss)
    return 0;

  double *prices = (double *) malloc(sizeof(double) * size);

  int err = calculate_bsmf(BSM_NUG_v, od, pd, size, Ss, pms, prices);

  if (err)
    return err;

  return result_set_prices(ret, prices);
}

static int greek_delta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double delta;
  int err = calculate_bsmf(BSM_NUG_delta, od, pd, 1, &S, pms, &delta);

  if (err)
    return err;

  return result_set_delta(ret, delta);
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double gamma;
  int err = calculate_bsmf(BSM_NUG_gamma, od, pd, 1, &S, pms, &gamma);

  if (err)
    return err;

  return result_set_gamma(ret, gamma);
}

static int greek_theta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double theta;
  int err = calculate_bsmf(BSM_NUG_theta, od, pd, 1, &S, pms, &theta);

  if (err)
    return err;

  return result_set_theta(ret, theta);
}

static int greek_rho(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  static const double delta = 0.0001;

  pricing_data pd0 = new_pricing_data_(pd);

  double r = rfr_get_value(pd0->r);

  rfr_set_value(pd0->r, r - delta);
  double f1;
  int err1 = calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f1);

  rfr_set_value(pd0->r, r + delta);
  double f2;
  int err2 = calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f2);

  double rho =  (f2 - f1) / (2 * delta);

  delete_pricing_data_(pd0);

  if (err1 || err2)
    return err1;

  return result_set_rho(ret, rho);
}

static int greek_vega(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  static const double delta = 0.0001;

  pricing_data pd0 = new_pricing_data_(pd);

  double vol = vol_get_value(pd0->vol);

  vol_set_value(pd0->vol, vol - delta);
  double f1;
  int err1 = calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f1);

  vol_set_value(pd0->vol, vol + delta);
  double f2;
  int err2 = calculate_bsmf(BSM_NUG_v, od, pd0, 1, &S, pms, &f2);

  double vega =  (f2 - f1) / (2 * delta);

  delete_pricing_data_(pd0);

  if (err1 || err2)
    return err1;

  return result_set_vega(ret, vega);
}

pricing_method new_american_finite_difference_non_uniform_grid(pricing_data pd) {
  return new_pricing_method_(option_price, price_precision, option_prices,
    greek_delta, greek_gamma, greek_theta, greek_rho, greek_vega, impl_vol,
    NULL, pd, NULL);
}
