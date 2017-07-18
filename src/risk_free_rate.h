#ifndef __RISK_FREE_RATE_H__
#define __RISK_FREE_RATE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct parameter * risk_free_rate;

risk_free_rate new_risk_free_rate(double val);
risk_free_rate new_risk_free_rate_(risk_free_rate rfr);

void delete_risk_free_rate(risk_free_rate rfr);

int rfr_set_value(risk_free_rate rfr, double val);
double rfr_get_value(risk_free_rate rfr);

#ifdef __cplusplus
}
#endif

#endif /* __RISK_FREE_RATE_H__ */
