#include <stdio.h>

#include <libop.h>

int main(int argc, char const *argv[]) {

  double S = 21.0, strike = 20.0;
  volatility sigma = 0.4;
  risk_free_rate r = 0.09;
  dividend d = 0.1;

  option eu_call_option = new_option(OPTION_CALL, EU_EXERCISE, YEAR, strike);

  pricing_method eur_an = new_pricing_method(EU_ANALYTIC, sigma, r, d);

  option_set_pricing_method(eu_call_option, eur_an);

  result result = new_result();

  option_price(eu_call_option, S, YEAR, result);

  printf("price = %lf\n", *result);

  return 0;
}
