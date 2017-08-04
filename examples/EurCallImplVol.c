#include <stdio.h>
#include <libop.h>

/**
 * Opción call europea con precio de strike $20 y tiempo a la expiración de un año.
 * Posee un dividendo continuo del 10% y una tasa libre de riesgo de 9%.
 * Se calcula la volatilidad implícita para un precio de subyacente de $21 y
 * un precio de opción de $3.773 calculado previamente.
 * La volatilidad implícita debería ser de 40%.
 */

int main(int argc, char const *argv[]) {

  double  underlying = 21,
          strike = 20;
  /* No es necesario el sigma */
  volatility sigma = NULL;

  risk_free_rate r = new_risk_free_rate(0.09);
  dividend d = new_continuous_dividend(0.1);

  double price = 3.773466;

  time_period tp = new_time_period_365d();

  option opt = new_option(OPTION_CALL, EU_EXERCISE, DAYS(tp, 365), strike);
  pricing_method pm = new_pricing_method(EU_ANALYTIC, sigma, r, d);

  option_set_pricing_method(opt, pm);

  result result = new_result();

  option_impl_vol(opt,price, underlying, result);

  double implied_vol = result_get_impl_vol(result);

  printf("Implied Volatility = %lf\n", implied_vol);

  delete_result(result);
  delete_time_period(tp);
  delete_pricing_method(pm);
  delete_risk_free_rate(r);
  delete_volatility(sigma);
  delete_dividend(d);
  delete_option(opt);
  return 0;
}
