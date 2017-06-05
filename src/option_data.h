#ifndef __OPTION_DATA_H__
#define __OPTION_DATA_H__

#include <date.h>

typedef enum {
  OPTION_CALL,
  OPTION_PUT
} option_type;

typedef enum {
  AM_EXERCISE,
  EU_EXERCISE
} exercise_type;

typedef struct option_data_ * option_data;

struct option_data_ {
  option_type   opt_type;
  exercise_type exercise;
  date          maturity;
  double        strike; /***/
};

option_data new_option_data(option_type, exercise_type, date, double);

void delete_option_data(option_data);

#endif /* __OPTION_DATA_H__ */
