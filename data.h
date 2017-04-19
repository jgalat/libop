#ifndef __DATA_H_
#define __DATA_H_

#include "date.h"
#include "result.h"

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

struct pricing_method_;

typedef struct pricing_method_ * pricing_method;


option new_option(option_type, exercise_type, date);

option_type option_get_ot(option);
exercise_type option_get_et(option);
date option_get_maturity(option);

void option_set_pricing_method(option, pricing_method);

int option_price(option, double S, date t, result);
// option greeks
// int option_delta(option, double S, date t, result);
// // ...
// int option_set_iv_method(option, void* ivm);
// int option_iv(option, double S, date t, double V, result);
//
// int option_clean(option);

#include "pricing_methods/pricing_methods.h"

pricing_method new_pricing_method(method_identifier, double sigma, double r, double d, double k);
pricing_method new_pricing_method_(int (*opt_price)(option, pricing_method, double, date, result),
                                    double sigma, double r, double d, double k);

double pm_get_sigma(pricing_method);
double pm_get_r(pricing_method);
double pm_get_d(pricing_method);
double pm_get_k(pricing_method);

void pm_set_sigma(pricing_method, double);
void pm_set_r(pricing_method, double);
void pm_set_d(pricing_method, double);
void pm_set_k(pricing_method, double);



//int pm_set_option_price_f(pricing_method, int (*)(option, pricing_method, double, date, result));


#endif /* __DATA_H_ */
