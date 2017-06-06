#include <stdio.h>

#include <libop.h>

int main(int argc, char const *argv[]) {

  double S = 21.0, strike = 20.0;
  volatility sigma = 0.4;
  risk_free_rate r = 0.09;
  dividend d = new_continuous_dividend(0.1);
  double option_value = 3.447474;

  int iv = 0, eur = 0;

  // dividend d = new_discrete_dividend();
  // div_disc_set_dates(d, 1, 0.5);
  // div_disc_set_ammounts(d, 1, 1.0);

  option opt;
  pricing_method pm;

  if (eur) {
    opt = new_option(OPTION_CALL, EU_EXERCISE, ONE_YEAR, strike);

    if (iv) {
      pm = new_pricing_method(EU_ANALYTIC, option_value, r, d);
    } else {
      pm = new_pricing_method(EU_ANALYTIC, sigma, r, d);
    }
  } else {
    opt = new_option(OPTION_CALL, AM_EXERCISE, ONE_YEAR, strike);

    if (iv) {
      pm = new_pricing_method(AM_FD, option_value, r, d);
    } else {
      pm = new_pricing_method(AM_FD, sigma, r, d);
    }
  }

  // pm_options pmo = new_pm_options();
  // pm_options_set_N(pmo, 50);
  // pm_set_options(pm, pmo);

  option_set_pricing_method(opt, pm);

  result result = new_result();

  if (iv) {
    option_impl_vol(opt, S, ONE_YEAR, result);
    printf("impl_vol = %lf\n", *result);
  } else {
    option_price(opt, S, ONE_YEAR, result);
    printf("V = %lf\n", *result);

    option_delta(opt, S, ONE_YEAR, result);
    printf("delta = %lf\n", *result);

    option_gamma(opt, S, ONE_YEAR, result);
    printf("gamma = %lf\n", *result);

    option_theta(opt, S, ONE_YEAR, result);
    printf("theta = %lf\n", *result);

    option_rho(opt, S, ONE_YEAR, result);
    printf("rho = %lf\n", *result);

    option_vega(opt, S, ONE_YEAR, result);
    printf("vega = %lf\n", *result);
  }

  delete_result(result);
  // delete_pm_options(pmo);
  delete_pricing_method(pm);
  delete_dividend(d);
  delete_option(opt);
  return 0;
}
