#include <string.h>
#include <stdlib.h>
#include <result_internal.h>
#include <option_data.h>
#include <option_data_internal.h>
#include <pricing_method_internal.h>
#include <qss-solver/BSMNonUniformGrid.h>
#include <impl_vol_methods/impl_vol_methods.h>
#include "american_finite_difference_non_uniform_grid.h"

pricing_method new_american_finite_difference_non_uniform_grid(pricing_data pd) {
  return new_pricing_method_(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pd, NULL);
}
