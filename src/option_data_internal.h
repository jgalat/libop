#ifndef __OPTION_DATA_INTERNAL_H__
#define __OPTION_DATA_INTERNAL_H__

#include <time_period.h>

typedef struct option_data_ * option_data;

struct option_data_ {
  option_type   opt_type;
  exercise_type exercise;
  time_period   maturity;
  double        strike; /***/
};

option_data new_option_data(option_type, exercise_type, time_period, double);

void delete_option_data(option_data);

#endif /* __OPTION_DATA_INTERNAL_H__ */
