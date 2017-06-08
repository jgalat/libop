#include <stdlib.h>

#include "time_period.h"

static const double _365 = 0.00273972602;
static const double _252 = 0.00396825396;

struct time_period_ {
  double t;
  double period;
};

time_period new_time_period(double p) {
  time_period tp = (time_period) malloc(sizeof(struct time_period_));
  if (tp) {
    tp->t = 0;
    tp->period = p;
  }
  return tp;
}

time_period new_time_period_365d() {
  return new_time_period(_365);
}

time_period new_time_period_252d() {
  return new_time_period(_252);
}

void delete_time_period(time_period tp) {
  free(tp);
}

int tp_set_days(time_period tp, int d) {
  if (!tp)
    return -1;
  double t = d * tp->period;
  tp->t = t;
  return 0;
}

int tp_set_years(time_period tp, int y) {
  if (!tp)
    return -1;
  tp->t = (double) y;
  return 0;
}

double tp_get_time(time_period tp) {
  return tp->t;
}

double tp_get_period(time_period tp) {
  return tp->period;
}
