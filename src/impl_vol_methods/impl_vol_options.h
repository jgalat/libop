#ifndef __IMPL_VOL_METHOD_OPTIONS_H__
#define __IMPL_VOL_METHOD_OPTIONS_H__

#include <common.h>

typedef struct impl_vol_options_ * impl_vol_options;

/* TODO check better name... no longer options */

struct impl_vol_options_ {
  /* impl_vol_f args */
  option_data od;
  pricing_data pd;
  double S;
  date ttl;
  pm_options pmo;
  void* pm_data;
};

impl_vol_options new_impl_vol_options(option_data od, pricing_data pd,
  double S, date ttl, pm_options pmo, void *pm_data);

void delete_impl_vol_options(impl_vol_options);

#endif /* __IMPL_VOL_METHOD_OPTIONS_H__ */
