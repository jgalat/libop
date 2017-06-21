#include <stdlib.h>
#include <stdio.h>
#include "time_period.h"

static const double _365 = 1 / 365.0;
static const double _260 = 1 / 260.0;

struct time_period_ {
  date t;
  double period;
};

time_period new_time_period_(double p) {
  time_period tp = (time_period) malloc(sizeof(struct time_period_));
  if (tp) {
    tp->t = 0;
    tp->period = p;
  }
  return tp;
}

time_period new_time_period(int days) {
  return new_time_period_(1.0 / (double) days);
}


time_period new_time_period_365d() {
  return new_time_period_(_365);
}

time_period new_time_period_260d() {
  return new_time_period_(_260);
}

void delete_time_period(time_period tp) {
  free(tp);
}

date tp_int_to_date(time_period tp, int days) {
  return (date) ((double) days * tp->period);
}

int tp_set_days(time_period tp, int d) {
  if (!tp)
    return -1;
  tp->t = tp_int_to_date(tp, d);
  return 0;
}

int tp_set_years(time_period tp, int y) {
  if (!tp)
    return -1;
  tp->t = (date) y;
  return 0;
}

date tp_get_date(time_period tp) {
  return tp->t;
}

double tp_get_period(time_period tp) {
  return tp->period;
}
