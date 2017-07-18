#include <parameter.h>
#include "volatility.h"

volatility new_volatility(double val) {
  return new_parameter(val);
}

volatility new_volatility_(volatility vol) {
  return new_parameter_(vol);
}

void delete_volatility(volatility vol) {
  delete_parameter(vol);
}

int vol_set_value(volatility vol, double val) {
  return param_set_value(vol, val);
}

double vol_get_value(volatility vol) {
  return param_get_value(vol);
}
