#ifndef __DIVIDEND_H__
#define __DIVIDEND_H__

#include <time_period.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enum of types of dividends.
 */
typedef enum {
  DIV_DISCRETE,
  DIV_CONTINUOUS
} dividend_type;

/**
 * Dividend structure data type.
 */
typedef struct dividend_ * dividend;
struct dividend_;

/**
 * Creates a new continuous dividend. The continuous dividend is in decimals.
 * On error returns NULL.
 * i.e.: new_continuous_dividend(0.1) for 10% continuous dividend.
 */
dividend new_continuous_dividend(double continuous_dividend);

/**
 * Creates a new empty discrete dividend.
 * On error returns NULL.
 */
dividend new_discrete_dividend();

/**
 * Deletes a dividend structure.
 */
void delete_dividend(dividend);

/**
 * Gets the type of the dividend structure.
 */
dividend_type div_get_type(dividend);

/**
 * Gets the value of a continuous dividend.
 * Returns -1 if a discrete dividend is given.
 */
double div_cont_get_val(dividend);

/**
 * Gets the number of dividend payments in the structure.
 * Returns -1 if a continuous dividend is given.
 */
int div_disc_get_n(dividend);

/**
 * Gets the array of dates (in double format) in the dividend structure.
 * Every value can be transformed into days with the original time_period.
 * Returns NULL if a continuous dividend is given, or if it is empty.
 */
date *div_disc_get_dates(dividend);

/**
 * Gets the array of ammounts in the dividend structure.
 * Returns NULL if a continuous dividend is given, or if it is empty.
 */
double *div_disc_get_ammounts(dividend);

/**
 * Sets the number of trading days from now for every dividend payment.
 * Variadic function version, the number of dividend payments must be specified (size)
 * A setting of data overrides the currently existing one.
 * i.e.:
 *  For a payment in 100 days and another in 200 days
 *  div_disc_set_dates(d, tp, 2, 100, 200);
 * Returns -1 on error
 */
int div_disc_set_dates(dividend, time_period, int size, ...);

/**
 * Sets the ammounts for every dividend payments. The order must be the same used
 * for the dates.
 * Variadic function version, the number ammounts must be specified (size)
 * A setting of data overrides the currently existing one.
 * i.e.:
 *  For a unique payment of $3
 *  div_disc_set_ammounts(d, tp, 1, 3);
 * On error returns -1.
 */
int div_disc_set_ammounts(dividend, int size, ...);

/**
 * Non-variadic versions of the functions described above.
 * On error returns -1.
 * A setting of data overrides the currently existing one.
 */
int div_disc_set_dates_(dividend, time_period, int*);
int div_disc_set_ammounts_(dividend, double*);

#ifdef __cplusplus
}
#endif

#endif /* __DIVIDEND_H__ */
