#include <math.h>
#include "secant_method.h"

int secant_method(impl_vol_mf ivmf, impl_vol_options ivo, result r) {
  if (!ivo)
    return -1;

  pm_options pmo = ivo->pmo;

  double *init = pm_options_get_iv_init(pmo);

  double x0 = init[0],
         x1 = init[1],
         x2 = -1,
         eps = pm_options_get_iv_eps(pmo);

  int i, maxit = pm_options_get_iv_max_it(pmo);

  double y0, y1, y2;

  option_data od = ivo->od;
  pricing_data pd = ivo->pd;
  double S = ivo->S;
  date ttl = ivo->ttl;
  void *pm_data = ivo->pm_data;

  y0 = ivmf(x0, od, pd, S, ttl, pmo, pm_data);
  y1 = ivmf(x1, od, pd, S, ttl, pmo, pm_data);

  for (i = 0; i < maxit; i++) {
    /* check if y1 =! y0!! */
    x2 = x1 - y1 * ((x1 - x0) / (y1 - y0));
    y2 = ivmf(x2, od, pd, S, ttl, pmo, pm_data);

    if (fabs(y2) < eps)
      break;

    x0 = x1;
    y0 = y1;
    x1 = x2;
    y1 = y2;
  }

  set_result(r, x2);
  return 0;
}
