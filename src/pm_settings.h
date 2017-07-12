#ifndef __PM_SETTINGS_H__
#define __PM_SETTINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pm_settings_ * pm_settings;
struct pm_settings_;

pm_settings new_pm_settings();
void delete_pm_settings(pm_settings);

int pm_settings_set_grid_size(pm_settings, int grid_size);
int pm_settings_set_Smax(pm_settings, double Smax);
int pm_settings_set_tol(pm_settings, double tol);
int pm_settings_set_abstol(pm_settings, double abstol);
int pm_settings_set_iv_max_it(pm_settings, int max_iterations);
int pm_settings_set_iv_eps(pm_settings, double epsilon);
int pm_settings_set_iv_init(pm_settings, double guess_vol0, double guess_vol1);
int pm_settings_set_extra_data(pm_settings, void *extra_data);

int pm_settings_get_grid_size(pm_settings);
double pm_settings_get_Smax(pm_settings);
double pm_settings_get_tol(pm_settings);
double pm_settings_get_abstol(pm_settings);
int pm_settings_get_iv_max_it(pm_settings);
double pm_settings_get_iv_eps(pm_settings);
double *pm_settings_get_iv_init(pm_settings);
void *pm_settings_get_extra_data(pm_settings);


#ifdef __cplusplus
}
#endif

#endif /* __PM_SETTINGS_H__ */
