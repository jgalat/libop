#include <stdlib.h>
#include "impl_vol_options.h"

struct impl_vol_options_ {
  double range[2];
  double eps;
  /* impl_vol_f args */
  option_data od;
  pricing_data pd;
  double S;
  date ttl;
  pm_options pmo;
  void* pm_data;
};

impl_vol_options new_impl_vol_options() {
  return ((impl_vol_options) malloc (sizeof(struct impl_vol_options_)));
}
