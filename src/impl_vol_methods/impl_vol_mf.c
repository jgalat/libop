#include <stdlib.h>
#include "impl_vol_mf.h"

impl_vol_mf_args new_impl_vol_mf_args(option_data od, pricing_data pd, double V,
  double S, date ttl, pm_settings pms, void *pm_data) {
  impl_vol_mf_args ivmfa = ((impl_vol_mf_args) malloc (sizeof(struct impl_vol_mf_args_)));
  if (ivmfa) {
    ivmfa->od = od;
    ivmfa->pd = pd;
    ivmfa->V = V;
    ivmfa->S = S;
    ivmfa->ttl = ttl;
    ivmfa->pms = pms;
    ivmfa->pm_data = pm_data;
  }
  return ivmfa;
}

void delete_impl_vol_mf_args(impl_vol_mf_args ivmfa) {
  free(ivmfa);
}
