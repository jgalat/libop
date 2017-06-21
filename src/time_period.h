#ifndef __TIME_PERIOD_H__
#define __TIME_PERIOD_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef double date;

typedef struct time_period_ * time_period;
struct time_period_;

time_period new_time_period(int);
time_period new_time_period_365d();
time_period new_time_period_260d();

void delete_time_period(time_period);

date tp_int_to_date(time_period, int);

int tp_set_days(time_period, int);
int tp_set_years(time_period, int);

date tp_get_date(time_period);
double tp_get_period(time_period);

#define DAYS(tp, i) ({  tp_set_days(tp, i); \
                        tp; })

#define YEARS(tp, i) ({ tp_set_years(tp, i); \
                        tp; })

#ifdef __cplusplus
}
#endif

#endif
