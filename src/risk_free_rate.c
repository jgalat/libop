#include <parameter.h>
#include "risk_free_rate.h"

risk_free_rate new_risk_free_rate(double val) {
  return new_parameter(val);
}

risk_free_rate new_risk_free_rate_(risk_free_rate rfr) {
  return new_parameter_(rfr);
}

void delete_risk_free_rate(risk_free_rate rfr) {
  delete_parameter(rfr);
}

int rfr_set_value(risk_free_rate rfr, double val) {
  return param_set_value(rfr, val);
}

double rfr_get_value(risk_free_rate rfr) {
  return param_get_value(rfr);
}
