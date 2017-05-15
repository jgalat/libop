#ifndef __OPTION_DATA_H__
#define __OPTION_DATA_H__

#include "date.h"

struct option_data_;
typedef struct option_data_ * option_data;

typedef enum {
  OPTION_CALL,
  OPTION_PUT
} option_type;

typedef enum {
  AM_EXERCISE,
  EU_EXERCISE
} exercise_type;

option_data new_option_data(option_type, exercise_type, date, double);

option_type od_get_option_type(option_data od);
exercise_type od_get_exercise_type(option_data od);
date od_get_maturity(option_data od);
double od_get_strike(option_data od);


#endif /* __OPTION_DATA_H__ */
