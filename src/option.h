#ifndef __OPTION_H__
#define __OPTION_H__

#include <common.h>
#include <pricing_method.h>

struct option_;
typedef struct option_ * option;

option new_option(option_type, exercise_type, date, double);

void delete_option(option);

int option_set_pricing_method(option, pricing_method);

int option_price(option, double, date, result);
int option_delta(option, double, date, result);
int option_gamma(option, double, date, result);
int option_theta(option, double, date, result);
int option_rho(option, double, date, result);
int option_vega(option, double, date, result);
int option_impl_vol(option, double, date, result); 

#endif /* __OPTION_H__ */
