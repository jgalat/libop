#include <stdlib.h>
#include "impl_vol_options.h"

impl_vol_options new_impl_vol_options(option_data od, pricing_data pd,
  double S, date ttl, pm_options pmo, void *pm_data) {
  impl_vol_options ivo = ((impl_vol_options) malloc (sizeof(struct impl_vol_options_)));
  if (ivo) {
    /* impl_vol_f args... */
    ivo->od = od;
    ivo->pd = pd;
    ivo->S = S;
    ivo->ttl = ttl;
    ivo->pmo = pmo;
    ivo->pm_data = pm_data;
  }
  return ivo;
}

void delete_impl_vol_options(impl_vol_options ivo) {
  free(ivo);
}
