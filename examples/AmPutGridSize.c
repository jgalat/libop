#include <stdio.h>
#include <libop.h>

/**
 * Opción put americana con precio de strike $100 y tiempo a la expiración de 182 días.
 * Posee un dividendo continuo del 10%, volatilidad de 25% y una tasa libre de
 * riesgo de 10%.
 * Se calcula el precio de la opción para un precio de subyacente de $100.
 * Aumentamos el tamaño de la grilla para obtener mayor precisión.
 */

int main(int argc, char const *argv[]) {

  double  underlying = 100,
          strike = 100;
  volatility sigma = new_volatility(0.25);
  risk_free_rate r = new_risk_free_rate(0.1);
  dividend d = new_continuous_dividend(0.1);

  time_period tp = new_time_period_365d();

  option opt = new_option(OPTION_PUT, AM_EXERCISE, DAYS(tp, 182), strike);

  /* Se usan diferencias finitas con una grilla no uniforme (AM_FD_NUG) */
  pricing_method pm = new_pricing_method(AM_FD_NUG, sigma, r, d);

  option_set_pricing_method(opt, pm);

  pm_settings pms = new_pm_settings();

  /* Establecemos una grilla de 300 puntos */
  pm_settings_set_grid_size(pms, 300);
  pm_set_settings(pm, pms);

  result result = new_result();

  option_price(opt, underlying, result);

  double price = result_get_price(result);

  printf("Option price = %lf\n", price);

  delete_result(result);
  delete_time_period(tp);
  delete_pricing_method(pm);
  delete_pm_settings(pms);
  delete_risk_free_rate(r);
  delete_volatility(sigma);
  delete_dividend(d);
  delete_option(opt);
  return 0;
}
