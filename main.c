#include <stdio.h>

#include "data.h"

int main(int argc, char const *argv[]) {

  option eu_call_option = new_option(OPTION_CALL, EU_EXERCISE, 1.0);

  pricing_method eur_an = new_pricing_method(EUROPEAN_ANALYTIC, 0.4, 0.09, 0.1, 20);

  option_set_pricing_method(eu_call_option, eur_an);

  result result = new_result();

  option_price(eu_call_option, 21.0, 1.0, result);

  printf("price = %lf\n", *result);
  
  return 0;
}
