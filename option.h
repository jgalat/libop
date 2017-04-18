#ifndef __OPTION_H_
#define __OPTION_H_

#include "date.h"
#include "result.h"
#include "pricing_method.h"

typedef enum {
  OPTION_CALL,
  OPTION_PUT
} option_type;

typedef enum {
  AM_EXERCISE,
  EU_EXERCISE
} exercise_type;

struct option_;

typedef struct option_ * option;

option new_option(option_type, exercise_type, date);
int option_set_pricing_method(option, pricing_method);

int option_price(option, double S, date t, result);
// option greeks
int option_delta(option, double S, date t, result);
// ...
int option_set_iv_method(option, void* ivm);
int option_iv(option, double S, date t, double V, result);

int option_clean(option);


#endif /* __OPTION_H_ */
