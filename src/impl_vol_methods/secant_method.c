#include <math.h>
#include <result_internal.h>
#include "secant_method.h"

int secant_method(impl_vol_mf ivmf, impl_vol_mf_args ivmfa, result r) {
  if (!ivmfa)
    return -1;

  pm_settings pms = ivmfa->pms;

  double init_[2];
  double *init = pm_settings_get_iv_init(pms, init_);

  if (!init)
    init = init_;

  double x0 = init[0],
         x1 = init[1],
         x2 = -1,
         eps = pm_settings_get_iv_eps(pms);

  int i, maxit = pm_settings_get_iv_max_it(pms);

  double y0, y1, y2;

  y0 = ivmf(x0, ivmfa);
  y1 = ivmf(x1, ivmfa);

  for (i = 0; i < maxit; i++) {
    /* check if y1 =! y0!! */
    x2 = x1 - y1 * ((x1 - x0) / (y1 - y0));
    y2 = ivmf(x2, ivmfa);

    if (fabs(y2) < eps)
      break;

    x0 = x1;
    y0 = y1;
    x1 = x2;
    y1 = y2;
  }

  return result_set_impl_vol(r, x2);
}
