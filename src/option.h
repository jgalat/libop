#ifndef __OPTION_H__
#define __OPTION_H__

#include <result.h>
#include <option_data.h>
#include <pricing_method.h>

#ifdef __cplusplus
extern "C" {
#endif

struct option_;
typedef struct option_ * option;

option new_option(option_type, exercise_type, time_period, double strike);

void delete_option(option);

int option_set_pricing_method(option, pricing_method);

int option_price(option, double underlying, result);
int option_delta(option, double underlying, result);
int option_gamma(option, double underlying, result);
int option_theta(option, double underlying, result);
int option_rho(option, double underlying, result);
int option_vega(option, double underlying, result);
int option_impl_vol(option, double price, double underlying, result);

/* TODO better name? */
int option_price_and_greeks(option, double underlying, result);

#ifdef __cplusplus
}
#endif

#endif /* __OPTION_H__ */
