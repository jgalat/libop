#ifndef __PRICING_METHODS_H__
#define __PRICING_METHODS_H__

#include <pricing_methods/european/european_analytic.h>
#include <pricing_methods/american/finite_difference/american_finite_difference.h>

typedef enum {
  EU_ANALYTIC,
  AM_FD
} method_id;

#endif /* __PRICING_METHODS_H__ */
