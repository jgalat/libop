#include <stdlib.h>
#include "option_data.h"

struct option_data_ {
  option_type   type;
  exercise_type exercise;
  date          maturity;
  double        strike; /***/
};

static int valid_option_type(option_type ot) {
  switch (ot) {
    case OPTION_CALL:
    case OPTION_PUT:
      return 1;
    default:
      return 0;
  }
}
static int valid_exercise_type(exercise_type et) {
  switch (et) {
    case AM_EXERCISE:
    case EU_EXERCISE:
      return 1;
    default:
      return 0;
  }
}

option_data new_option_data(option_type t, exercise_type e, date m, double strike) {
  if (!valid_option_type(t) || !valid_exercise_type(e))
    return NULL;

  option_data od = (option_data) malloc(sizeof(struct option_data_));
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
