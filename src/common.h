#ifndef __COMMON_H__
#define __COMMON_H__

#include <date.h>
#include <result.h>
#include <parameter.h>
#include <option_data.h>
#include <pricing_data.h>

typedef int (*price_function) (option_data, pricing_data, double S, date, result);

#endif /* __COMMON_H__ */
