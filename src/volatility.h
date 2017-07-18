#ifndef __VOLATILITY_H__
#define __VOLATILITY_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct parameter * volatility;

volatility new_volatility(double val);
volatility new_volatility_(volatility vol);

void delete_volatility(volatility vol);

int vol_set_value(volatility vol, double val);
double vol_get_value(volatility vol);

#ifdef __cplusplus
}
#endif

#endif /* __VOLATILITY_H__ */
