#include <stdio.h>

#include <libop.h>

int main(int argc, char const *argv[]) {

  double S = 21, strike = 20;
  volatility sigma = new_volatility(0.4);
  risk_free_rate r = new_risk_free_rate(0.09);
  dividend d = new_continuous_dividend(0.1);
  double option_value = 3.447474;

  time_period tp = new_time_period_365d();

  int iv = 0 , eur = 0;

  // dividend d = new_discrete_dividend();
  // int size_dd = 1;
  // int dates[2] = { 182 };
  // double ammounts[2] = { 7 };
  //
  // div_disc_set_dates_(d, tp, size_dd, dates);
  // div_disc_set_ammounts_(d, size_dd, ammounts);

  option opt;
  pricing_method pm;

  if (eur) {
    opt = new_option(OPTION_CALL, EU_EXERCISE, YEARS(tp, 1), strike);
    pm = new_pricing_method(EU_ANALYTIC, sigma, r, d);
  } else {
    opt = new_option(OPTION_CALL, AM_EXERCISE, DAYS(tp, 365), strike);
    pm = new_pricing_method(AM_FD_UG, sigma, r, d);
  }

  pm_settings pms = new_pm_settings();
  // pm_settings_set_grid_size(pms, 100);
  pm_set_settings(pm, pms);

  option_set_pricing_method(opt, pm);

  result result = new_result();

  if (iv) {
    option_impl_vol(opt, option_value, S, result);
    printf("impl_vol = %lf\n", result_get_impl_vol(result));
  } else {
    option_price(opt, S, result);

    double price = result_get_price(result);
    option_price_precision(opt, price, S, result);

    printf("precision = %lf\n", result_get_price_precision(result));

    // printf("V = %lf\n", result_get_price(result));
    // printf("delta = %lf\n", result_get_delta(result));
    // printf("gamma = %lf\n", result_get_gamma(result));
    // printf("theta = %lf\n", result_get_theta(result));
    // printf("rho = %lf\n", result_get_rho(result));
    // printf("vega = %lf\n", result_get_vega(result));
  }

  delete_result(result);
  delete_pm_settings(pms);
  delete_time_period(tp);
  delete_pricing_method(pm);
  delete_risk_free_rate(r);
  delete_volatility(sigma);
  delete_dividend(d);
  delete_option(opt);
  return 0;
}
