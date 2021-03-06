#ifndef __PRICING_METHOD_H__
#define __PRICING_METHOD_H__

#include <risk_free_rate.h>
#include <volatility.h>
#include <dividend.h>
#include <pm_settings.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pricing method strucure data type.
 */
struct pricing_method_;
typedef struct pricing_method_ * pricing_method;

/**
 * Enum with the current pricing methods available.
 */
typedef enum {
  EU_ANALYTIC,
  AM_FD_UG,
  AM_FD_NUG
} method_id;

/**
 * Creates a new pricing method structure of the method_id given.
 * On error returns NULL.
 */
pricing_method new_pricing_method(method_id, volatility, risk_free_rate, dividend);

/**
 * Deletes a pricing method structure.
 */
void delete_pricing_method(pricing_method);

/**
 * Sets a pm_settings (pm_settings.h) to the pricing method.
 * On error returns -1.
 */
int pm_set_settings(pricing_method, pm_settings);

#ifdef __cplusplus
}
#endif

#endif /* __PRICING_METHOD_H__ */
