#ifndef __DIVIDEND_H__
#define __DIVIDEND_H__

#include <time_period.h>

typedef enum {
  DIV_DISCRETE,
  DIV_CONTINUOUS
} dividend_type;

typedef struct dividend_ * dividend;
struct dividend_;

dividend new_continuous_dividend(double);
dividend new_discrete_dividend();

void delete_dividend(dividend);

dividend_type div_get_type(dividend);

double div_cont_get_val(dividend);

int div_disc_get_n(dividend);
date *div_disc_get_dates(dividend);
double *div_disc_get_ammounts(dividend);

int div_disc_set_dates(dividend, int, ...);
int div_disc_set_ammounts(dividend, int, ...);

#endif /* __DIVIDEND_H__ */
