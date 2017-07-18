#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#ifdef __cplusplus
extern "C" {
#endif

struct parameter;
struct parameter *new_parameter(double val);
struct parameter *new_parameter_(struct parameter *p);

void delete_parameter(struct parameter *p);

int param_set_value(struct parameter *p, double val);
double param_get_value(struct parameter *p);

#ifdef __cplusplus
}
#endif

#endif /* __PARAMETER_H__ */
