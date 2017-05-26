#include <stdlib.h>

#include "common.h"
#include "option.h"

struct option_ {
  option_data     option_data;
  pricing_method  pm;
};

option new_option(option_type ot, exercise_type et, date m, double k) {
  option o = (option) malloc(sizeof(struct option_));
  if (o) {
    o->option_data = new_option_data(ot, et, m, k);
    if (!o->option_data) {
      free(o);
      return NULL;
    }
    o->pm = NULL;
  }
  return o;
}

void delete_option(option o) {
  free(o);
}

int option_set_pricing_method(option o, pricing_method pm) {
  /** null */
  o->pm = pm;
  return 0;
}


int option_price(option o, double S, date ttl, result r) {
  return pm_option_price(o->pm, o->option_data, S, ttl, r);
}
