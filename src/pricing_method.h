#ifndef __PRICING_METHOD_H__
#define __PRICING_METHOD_H__

#include <parameter.h>
#include <dividend.h>
#include <pm_settings.h>

#ifdef __cplusplus
extern "C" {
#endif

struct pricing_method_;
typedef struct pricing_method_ * pricing_method;

typedef enum {
  EU_ANALYTIC,
  AM_FD
} method_id;

pricing_method new_pricing_method(method_id, volatility, risk_free_rate, dividend);

void delete_pricing_method(pricing_method);

int pm_set_settings(pricing_method, pm_settings);

#ifdef __cplusplus
}
#endif

#endif /* __PRICING_METHOD_H__ */
