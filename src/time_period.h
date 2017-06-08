#ifndef __TIME_PERIOD_H__
#define __TIME_PERIOD_H__

typedef struct time_period_ * time_period;
struct time_period;

time_period new_time_period(double);
time_period new_time_period_365d();
time_period new_time_period_252d();

void delete_time_period(time_period);

int tp_set_days(time_period, int);
int tp_set_years(time_period, int);

double tp_get_time(time_period);
double tp_get_period(time_period);

#define DAYS(tp, i) ({  tp_set_days(tp, i); \
                        tp; })

#define YEARS(tp, i) ({ tp_set_years(tp, i); \
                        tp; })

#endif
