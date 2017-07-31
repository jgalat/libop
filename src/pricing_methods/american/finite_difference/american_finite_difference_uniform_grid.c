#include <string.h>
#include <stdlib.h>
#include <result_internal.h>
#include <option_data.h>
#include <option_data_internal.h>
#include <pricing_method_internal.h>
#include <qss-solver/engine/common/utils.h>
#include <qss-solver/BSMUniformGrid.h>
#include <impl_vol_methods/impl_vol_methods.h>
#include <debug.h>

#include "american_finite_difference_uniform_grid.h"

#define SMAX_MULTIPLIER 5
#define N_IP_POINTS 4
#define N_SIM 2

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

static BSM_UG BSM_UG_(int grid_size, int discdiv_iter, date maturity,
  double Smax, double tol, double abstol, option_data od, pricing_data pd) {

  option_type type = od->opt_type;
  BSM_OT ot = option_type_BSM_OT(type);
  time_period tp = od->maturity;
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
    dd_n = MIN(discdiv_iter, div_disc_get_n(divi));
    dd_d = div_disc_get_dates(divi);
    dd_a = div_disc_get_ammounts(divi);
  }

  return new_BSM_UG(grid_size, ot, Smax, sigma, r, K, d, dd_n, dd_d, dd_a, period,
    maturity, tol, abstol);
}

static double query_value(BSM_UG *bsm, BSM_UG_F bsmf, double S, int N, double Smax) {
  int i;
  double y50[N_IP_POINTS], y100[N_IP_POINTS], s[N_IP_POINTS],
    y[N_IP_POINTS];

  double ds = Smax / ((double) N);
  int n = (int) ((double) N / Smax * S);
  int p50[N_IP_POINTS] = { n-1, n, n+1, n+2 };
  int p100[N_IP_POINTS] = { 2*n-2, 2*n, 2*n+2, 2*n+4 };

  for (i = 0; i < N_IP_POINTS; i++) {
    s[i] = ds * p50[i];
    y50[i] = bsmf(bsm[0], p50[i]);
    y100[i] = bsmf(bsm[1], p100[i]);
    /* richardson */
    y[i] = (4.0 * y100[i] - y50[i]) / 3.0;
  }

  return lagrange_interpolation(S, s, y, N_IP_POINTS);
}

static void __apply_div(int discdiv_iter, double *ammounts, int size, double *S) {
  int i, j;
  for (i = 0; i < size; i++)
    for (j = 0; j < discdiv_iter; j++)
      S[i] -= ammounts[j];
}

static int calculate_bsmf(BSM_UG_F bsmf, option_data od, pricing_data pd,
  int size, double *Ss, pm_settings pms, double *output) {

  int N_ = pm_settings_get_grid_size(pms);
  int halfN = N_ / 2;
  int N[2] = { halfN, halfN * 2 };
  double tol = pm_settings_get_tol(pms);
  double abstol = pm_settings_get_abstol(pms);

  int i;
  double K = od->strike,
         Smax = pm_settings_get_Smax(pms);

  date maturity = tp_get_date(od->maturity);

  if (Smax < 0)
    Smax = SMAX_MULTIPLIER * K;

  BSM_UG *bsm = NULL;
  int bsm_deletes = 0;

  switch (div_get_type(pd->d)) {
    case DIV_CONTINUOUS:
      bsm = malloc(sizeof(BSM_UG) * N_SIM);
      bsm_deletes = N_SIM;
      for (i = 0; i < N_SIM; i++) {
        bsm[i] = BSM_UG_(N[i], 0, maturity, Smax, tol, abstol, od, pd);
      }
      for(i = 0; i < size; i++)
        output[i] = query_value(bsm, bsmf, Ss[i], halfN, Smax);
      break;
    case DIV_DISCRETE:
      i = 0;
      int discdiv_max = div_disc_get_n(pd->d), discdiv_iter;
      date *discdiv_dates = div_disc_get_dates(pd->d);
      double *ammounts = div_disc_get_ammounts(pd->d);

      double *Ss_ = (double *) malloc(sizeof(double) * size);

      int *max_v = malloc(sizeof(int) * (discdiv_max + 1));
      double **possible_output = malloc(sizeof(double*) * size);
      for (i = 0; i < size; i++)
        possible_output[i] = malloc(sizeof(double) * (discdiv_max + 1));

      bsm = malloc(sizeof(BSM_UG) * N_SIM * (discdiv_max + 1));
      bsm_deletes = N_SIM * (discdiv_max + 1);

      /* Iteration over dividends, paying or not */
      for (discdiv_iter = 0; discdiv_iter < discdiv_max; discdiv_iter++) {
        for (i = 0; i < N_SIM; i++){
          bsm[N_SIM * discdiv_iter + i] =
            BSM_UG_(N[i], discdiv_iter, discdiv_dates[discdiv_iter],
              Smax, tol, abstol, od, pd);
        }
      }

      /* Paying all the dividends */
      for (i = 0; i < N_SIM; i++) {
        bsm[N_SIM * discdiv_max + i] =
          BSM_UG_(N[i], discdiv_max, maturity, Smax, tol, abstol, od, pd);
      }

      for (discdiv_iter = 0; discdiv_iter < discdiv_max + 1; discdiv_iter++) {
        memcpy(Ss_, Ss, sizeof(double) * size);
        __apply_div(discdiv_iter, ammounts, size, Ss_);
        for (i = 0; i < size; i++) {
          possible_output[i][discdiv_iter] =
            query_value(&bsm[N_SIM * discdiv_iter], BSM_UG_v, Ss_[i], halfN, Smax);
        }
      }

      for (i = 0; i < size; i++) {
        max_v[i] = 0;
        for (discdiv_iter = 1; discdiv_iter < discdiv_max + 1; discdiv_iter++) {
          if (possible_output[i][discdiv_iter] > possible_output[i][max_v[i]]) {
            max_v[i] = discdiv_iter;
          }
        }
      }

      if (bsmf == BSM_UG_v) {
        for (i = 0; i < size; i++)
          output[i] = possible_output[i][max_v[i]];
      } else {
        for (i = 0; i < size; i++) {
          memcpy(Ss_, Ss, sizeof(double) * size);
          __apply_div(max_v[i], ammounts, size, Ss_);
          output[i] =
            query_value(&bsm[N_SIM * max_v[i]], bsmf, Ss_[i], halfN, Smax);
        }
      }

      free(max_v);
      free(Ss_);
      for (i = 0; i < size; i++)
        free(possible_output[i]);
      free(possible_output);
      break;
  }

  for (i = 0; i < bsm_deletes; i++) {
    delete_BSM_UG(bsm[i]);
  }
  free(bsm);
  return 0;
}

static double iv_f(double vol, impl_vol_mf_args ivmfa) {
  pricing_data pd = ivmfa->pd;
  vol_set_value(pd->vol, vol);
  double price;
  calculate_bsmf(BSM_UG_v, ivmfa->od, pd, 1, &ivmfa->S, ivmfa->pms, &price);
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
  calculate_bsmf(BSM_UG_v, od, pd, 1, &S, pms, &price);

  return result_set_price(ret, price);
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

  calculate_bsmf(BSM_UG_v, od, pd, size, Ss, pms, prices);

  return result_set_prices(ret, prices);
}

static int greek_delta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double delta;
  calculate_bsmf(BSM_UG_delta, od, pd, 1, &S, pms, &delta);

  return result_set_delta(ret, delta);
}

static int greek_gamma(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double gamma;
  calculate_bsmf(BSM_UG_gamma, od, pd, 1, &S, pms, &gamma);

  return result_set_gamma(ret, gamma);
}

static int greek_theta(option_data od, pricing_data pd, double S,
  result ret, pm_settings pms, void *pm_data) {

  if (od->exercise != AM_EXERCISE) {
    __DEBUG(__OPT_NOT_AM);
    return -1;
  }

  double theta;
  calculate_bsmf(BSM_UG_theta, od, pd, 1, &S, pms, &theta);

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
  calculate_bsmf(BSM_UG_v, od, pd0, 1, &S, pms, &f1);

  rfr_set_value(pd0->r, r + delta);
  double f2;
  calculate_bsmf(BSM_UG_v, od, pd0, 1, &S, pms, &f2);

  double rho =  (f2 - f1) / (2 * delta);

  delete_pricing_data_(pd0);
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
  calculate_bsmf(BSM_UG_v, od, pd0, 1, &S, pms, &f1);

  vol_set_value(pd0->vol, vol + delta);
  double f2;
  calculate_bsmf(BSM_UG_v, od, pd0, 1, &S, pms, &f2);

  double vega =  (f2 - f1) / (2 * delta);

  delete_pricing_data_(pd0);
  return result_set_vega(ret, vega);
}

pricing_method new_american_finite_difference_uniform_grid(pricing_data pd) {
  return new_pricing_method_(option_price, NULL, option_prices, greek_delta,
    greek_gamma, greek_theta, greek_rho, greek_vega, impl_vol, NULL, pd, NULL);
}

#undef SMAX_MULTIPLIER
#undef N_IP_POINTS
#undef N_SIM
