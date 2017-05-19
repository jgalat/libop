#include <stdlib.h>
#include <qss-solver/black_scholes_model.h>
#include "american_finite_difference.h"


int american_finite_difference_option_price(option_data od, pricing_data pd,
  double S, date ttl, result ret) {
  
  black_scholes_model bsm = new_black_scholes_model();
  black_scholes_model_testrun(bsm);
  return 0;
}

pricing_method new_american_finite_difference(pricing_data pd) {
  return new_pricing_method_(american_finite_difference_option_price, pd);
}
