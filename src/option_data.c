#include <stdlib.h>
#include "option_data.h"
#include "option_data_internal.h"

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

option_data new_option_data(option_type t, exercise_type e, time_period m,
  double strike) {
  if (!valid_option_type(t) || !valid_exercise_type(e))
    return NULL;

  if (!m)
    return NULL;

  option_data od = (option_data) malloc(sizeof(struct option_data_));
  if (od) {
    od->opt_type = t;
    od->exercise = e;
    od->maturity = m;
    od->strike = strike;
  }
  return od;
}

void delete_option_data(option_data od) {
  free(od);
}
