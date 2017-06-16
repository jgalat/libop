#ifndef __OPTION_H__
#define __OPTION_H__

#include <result.h>
#include <option_data.h>
#include <pricing_method.h>

struct option_;
typedef struct option_ * option;

option new_option(option_type, exercise_type, time_period, double);

void delete_option(option);

int option_set_pricing_method(option, pricing_method);

int option_price(option, double, result);
int option_delta(option, double, result);
int option_gamma(option, double, result);
int option_theta(option, double, result);
int option_rho(option, double, result);
int option_vega(option, double, result);
int option_impl_vol(option, double, double, result);

/* TODO better name? */
int option_price_and_greeks(option, double, result);

#endif /* __OPTION_H__ */
