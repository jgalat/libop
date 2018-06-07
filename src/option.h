#ifndef __OPTION_H__
#define __OPTION_H__

#include <result.h>
#include <option_data.h>
#include <pricing_method.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Option structure data type.
 */
typedef struct option_ * option;
struct option_;

/**
 * Creates a new option.
 * On error returns NULL.
 */
option new_option(option_type, exercise_type, time_period, double strike);

/**
 * Deletes an option structure.
 */
void delete_option(option);

/**
 * Sets a given pricing method to a specified option.
 * Option structures require pricing methods to valuate prices and greeks.
 * On error returns -1.
 * More on pricing methods in pricing_method.h.
 */
int option_set_pricing_method(option, pricing_method);

/**
 * Calculates the price of an option for a given underlying.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_price(option, double underlying, result);

/**
 * Calculates the precision of the last price returned
 * It requires a result structure (result.h) to store the result.
 * The precision calculation method must be implemented in the already set
 * pricing method.
 * On error returns -1.
 */
int option_price_precision(option, double price, double underlying, result);

/**
 * Calculates the prices of an option for each value of the underlying given.
 * Size of the underlying array must be specified.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_prices(option, int size, double *underlyng, result);

/**
 * Calculates the delta of an option for a given underlying.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_delta(option, double underlying, result);

/**
 * Calculates the gamma of an option for a given underlying.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_gamma(option, double underlying, result);

/**
 * Calculates the theta of an option for a given underlying.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_theta(option, double underlying, result);

/**
 * Calculates the rho of an option for a given underlying.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_rho(option, double underlying, result);

/**
 * Calculates the vega of an option for a given underlying.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 */
int option_vega(option, double underlying, result);

/**
 * Calculates the price and the greeks of a given option.
 * It requires a result structure (result.h) to store the result.
 * On error returns -1.
 * Equivalent to run the functions described above.
 */
int option_price_and_greeks(option, double underlying, result);

/**
 * Given an option price and underlying, calculates the implied volatilty.
 * It requires a 'result' structure to store the result.
 * On error returns -1.
 */
int option_impl_vol(option, double price, double underlying, result);

#ifdef __cplusplus
}
#endif

#endif /* __OPTION_H__ */
