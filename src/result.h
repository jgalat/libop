#ifndef __RESULT_H__
#define __RESULT_H__

#include <parameter.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Result strucure data type.
 */
typedef struct result_ * result;
struct result_;

/**
 * Creates a new empty result structure.
 * On error returns NULL.
 */
result new_result();

/**
 * Deletes a result structure.
 */
void delete_result(result);

/**
 * Get functions to access the data stored by pricing methods.
 */
double result_get_price(result);
double *result_get_prices(result);
double result_get_delta(result);
double result_get_gamma(result);
double result_get_theta(result);
double result_get_rho(result);
double result_get_vega(result);
volatility result_get_impl_vol(result);

#ifdef __cplusplus
}
#endif

#endif /* __RESULT_H__ */
