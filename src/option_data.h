#ifndef __OPTION_DATA_H__
#define __OPTION_DATA_H__

#include <date.h>

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

void delete_option_data(option_data);

option_type od_get_option_type(option_data);
exercise_type od_get_exercise_type(option_data);
date od_get_maturity(option_data);
double od_get_strike(option_data);


#endif /* __OPTION_DATA_H__ */
