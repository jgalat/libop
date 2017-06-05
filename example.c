#include <stdio.h>

#include <libop.h>

int main(int argc, char const *argv[]) {

  double S = 21.0, strike = 20.0;
  volatility sigma = 0.4;
  risk_free_rate r = 0.09;
  dividend d = new_continuous_dividend(0.1);
  double option_value = 3.447474;

  // dividend d = new_discrete_dividend();
  // div_disc_set_dates(d, 1, 0.5);
  // div_disc_set_ammounts(d, 1, 1.0);

  // option eu_call = new_option(OPTION_CALL, EU_EXERCISE, YEAR, strike);

  option am_call = new_option(OPTION_CALL, AM_EXERCISE, YEAR, strike);

  // pricing_method eur_an = new_pricing_method(EU_ANALYTIC, sigma, r, d);

  // pricing_method eur_an = new_pricing_method(EU_ANALYTIC, option_value, r, d);

  // pricing_method am_fd = new_pricing_method(AM_FD, sigma, r, d);

  pricing_method am_fd = new_pricing_method(AM_FD, option_value, r, d);

  // pm_options pmo = new_pm_options();
  // pm_options_set_N(pmo, 50);
  // pm_set_options(am_fd, pmo);

  // option_set_pricing_method(eu_call, eur_an);

  option_set_pricing_method(am_call, am_fd);

  result result = new_result();

  // option_price(eu_call, S, 0 /* no need */, result);

  // option_price(am_call, S, YEAR, result);

  // option_impl_vol(eu_call, S, YEAR, result);

  option_impl_vol(am_call, S, YEAR, result);

  printf("result = %lf\n", *result);

  delete_result(result);
  // delete_pm_options(pmo);
  delete_pricing_method(am_fd);
  // delete_pricing_method(eur_an);
  delete_dividend(d);
  delete_option(am_call);
  // delete_option(eu_call);
  return 0;
}
