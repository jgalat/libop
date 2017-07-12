#ifndef __TIME_PERIOD_H__
#define __TIME_PERIOD_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef double date;

/**
 * Time period structure data type.
 */
typedef struct time_period_ * time_period;
struct time_period_;

/**
 * Creates a new time period structure given the number of trading days in a
 * year.
 * On error returns NULL.
 */
time_period new_time_period(int days_per_year);

/**
 * Equivalent to new_time_period(365).
 */
time_period new_time_period_365d();

/**
 * Equivalent to new_time_period(252).
 */
time_period new_time_period_252d();

/**
 * Deletes a time period structure.
 */
void delete_time_period(time_period);

/**
 * Transform a number of days (i.e. till expiration) to a double format.
 */
date tp_int_to_date(time_period, int days);

/**
 * Transform a date into a number of days.
 */
int tp_date_to_int(time_period, date date);

/**
 * Sets the period of trading days.
 * i.e.: For a week would be tp_set_days(tp, 7);
 * Overrides the current number.
 */
int tp_set_days(time_period, int days);

/**
 * Sets the period of trading days in years.
 * i.e.: For a year would be tp_set_years(tp, 1);
 * Overrides the current number.
 * It is prefered to use tp_set_days.
 * For a time period 'tp' created with a 365 days per year calendar
 * tp_set_years(tp, 1) == tp_set_days(tp, 365)
 */
int tp_set_years(time_period, int years);

date tp_get_date(time_period);
double tp_get_period(time_period);

/**
 * Macros defined to make reading easier.
 */
#define DAYS(tp, i) ({  tp_set_days(tp, i); \
                        tp; })

#define YEARS(tp, i) ({ tp_set_years(tp, i); \
                        tp; })

#ifdef __cplusplus
}
#endif

#endif
