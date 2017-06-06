#include <stdlib.h>
#include "impl_vol_mf.h"

impl_vol_mf_args new_impl_vol_mf_args(option_data od, pricing_data pd,
  double S, date ttl, pm_options pmo, void *pm_data) {
  impl_vol_mf_args ivmfa = ((impl_vol_mf_args) malloc (sizeof(struct impl_vol_mf_args_)));
  if (ivmfa) {
    ivmfa->od = od;
    ivmfa->pd = pd;
    ivmfa->S = S;
    ivmfa->ttl = ttl;
    ivmfa->pmo = pmo;
    ivmfa->pm_data = pm_data;
  }
  return ivmfa;
}

void delete_impl_vol_mf_args(impl_vol_mf_args ivmfa) {
  free(ivmfa);
}
