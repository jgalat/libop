#include <stdlib.h>
#include "option_data.h"

struct option_data_ {
  option_type   type;
  exercise_type exercise;
  date          maturity;
  double        strike; /***/
};

option_data new_option_data(option_type t, exercise_type e, date m, double strike) {
  option_data od = malloc(sizeof(struct option_data_));
  if (od) {
    od->type = t;
    od->exercise = e;
    od->maturity = m;
    od->strike = strike;
  }
  return od;
}

option_type od_get_option_type(option_data od) {
  return od->type;
}

exercise_type od_get_exercise_type(option_data od) {
  return od->exercise;
}

date od_get_maturity(option_data od) {
  return od->maturity;
}

double od_get_strike(option_data od) {
  return od->strike;
}
