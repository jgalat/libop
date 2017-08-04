#include <stdio.h>
#include <libop.h>

/**
 * Opción call americana con precio de strike $100 y tiempo a la expiración de 252 días.
 * Utilizando un año de 252 días.
 * Posee una volatilidad de 25% y una tasa libre de riesgo de 10%.
 * Posee dividendos discretos a pagar:
 * Tiempo hasta el pago - Monto
 *   50 días               10
 *   200 días              15
 * Se calcula el precio de la opción para un precio de subyacente de $100.
 */

int main(int argc, char const *argv[]) {

  double  underlying = 100,
          strike = 100;
  volatility sigma = new_volatility(0.25);
  risk_free_rate r = new_risk_free_rate(0.1);
  dividend d = new_discrete_dividend();

  time_period tp = new_time_period_252d();

  div_disc_set_dates(d, tp, 2, 50, 200);
  div_disc_set_ammounts(d, 2, 10, 15);

  option opt = new_option(OPTION_CALL, AM_EXERCISE, DAYS(tp, 182), strike);

  /* Se usan diferencias finitas con una grilla uniforme (AM_FD_UG) */
  pricing_method pm = new_pricing_method(AM_FD_UG, sigma, r, d);

  option_set_pricing_method(opt, pm);

  result result = new_result();

  option_price(opt, underlying, result);

  double price = result_get_price(result);

  printf("Option price = %lf\n", price);

  delete_result(result);
  delete_time_period(tp);
  delete_pricing_method(pm);
  delete_risk_free_rate(r);
  delete_volatility(sigma);
  delete_dividend(d);
  delete_option(opt);
  return 0;
}
