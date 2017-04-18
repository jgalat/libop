#include <stdlib.h>

#include "option.h"

struct option_ {
  option_type option_type;
  exercise_type exercise_type;
  date maturity;
};

option new_option(option_type ot, exercise_type et, date maturity) {
  option o = malloc(sizeof(option));
  o->option_type = ot;
  o->exercise_type = et;
  o->maturity = maturity;
  return o;
}
