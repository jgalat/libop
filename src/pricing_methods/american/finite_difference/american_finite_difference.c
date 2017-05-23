#include <stdlib.h>
#include <qss-solver/black_scholes_model.h>
#include "american_finite_difference.h"


int american_finite_difference_option_price(option_data od, pricing_data pd,
  double S, date ttl, result ret, void *pm_data) {

  // (int grid_size, BSM_OT ot, double smax,
  //   double vol, double rfr, double strike, double cont_div, int discdiv_n,
  //   double *discdiv_date, double *discdiv_ammo,
  //   double end_time, double tol, double abs_tol)

  //test
  black_scholes_model bsm = new_black_scholes_model(100, CALL, 100, 0.4, 0.09,
    20, 0.1, 0, NULL, NULL, 1, 1e-9, 1e-12);

  black_scholes_model_testrun(bsm);

  return 0;
}

pricing_method new_american_finite_difference(pricing_data pd) {
  return new_pricing_method_(american_finite_difference_option_price, pd, NULL);
}
