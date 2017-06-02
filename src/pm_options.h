#ifndef __PM_OPTIONS_H__
#define __PM_OPTIONS_H__

typedef struct pm_options_ * pm_options;
struct pm_options_;

pm_options new_pm_options();
void delete_pm_options(pm_options);

int pm_options_set_N(pm_options, int);
int pm_options_set_tol(pm_options, double);
int pm_options_set_abstol(pm_options, double);

int pm_options_get_N(pm_options);
double pm_options_get_tol(pm_options);
double pm_options_get_abstol(pm_options);


#endif /* __PM_OPTIONS_H__ */
