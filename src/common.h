#ifndef __COMMON_H__
#define __COMMON_H__

#include <date.h>
#include <result.h>
#include <parameter.h>
#include <dividend.h>
#include <option_data.h>
#include <pricing_data.h>

typedef int (*common_f) (option_data, pricing_data, double S, date,
  result, void*);

typedef common_f price_f;
typedef common_f delta_f;
typedef common_f gamma_f;
typedef common_f theta_f;
typedef common_f rho_f;
typedef common_f vega_f;

typedef void (*delete_f) (void*);

#endif /* __COMMON_H__ */
