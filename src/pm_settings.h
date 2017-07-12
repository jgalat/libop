#ifndef __PM_SETTINGS_H__
#define __PM_SETTINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pricing method settings structure data type.
 */
typedef struct pm_settings_ * pm_settings;
struct pm_settings_;

/**
 * Creates a new pricing method settings structure with default values.
 * Default values can be found in the body function in pm_settings.c.
 * On error returns NULL.
 */
pm_settings new_pm_settings();

/**
 * Deletes a pricing method settings structure.
 */
void delete_pm_settings(pm_settings);

/**
 * Sets the grid size used by the finite difference method.
 * Specifying a grid size of N will result in 2 grids of size N and 2N being used.
 * On error returns -1.
 */
int pm_settings_set_grid_size(pm_settings, int grid_size);

/**
 * Sets the max value of the underlying used by the finite difference method.
 * Values in the grid are contained in [0, Smax].
 * On error returns -1.
 */
int pm_settings_set_Smax(pm_settings, double Smax);

/**
 * Sets the tolerance used by the finite difference method.
 * On error returns -1.
 */
int pm_settings_set_tol(pm_settings, double tol);

/**
 * Sets the absolute tolerance used by the finite difference method.
 * On error returns -1.
 */
int pm_settings_set_abstol(pm_settings, double abstol);

/**
 * Sets the maximum number of iterations to be done by the implied volatility
 * calculation method.
 * On error returns -1.
 */
int pm_settings_set_iv_max_it(pm_settings, int max_iterations);

/**
 * Sets the ... to be used by the implied volatility calculation method.
 * On error returns -1.
 */
int pm_settings_set_iv_eps(pm_settings, double epsilon);

/**
 * Sets the volatility initial guesses used by the implied volatility
 * calculation method. The root doesn't need to be contained.
 * On error returns -1.
 */
int pm_settings_set_iv_init(pm_settings, double guess_vol0, double guess_vol1);

/**
 * Sets a custom field with extra_data that wasn't covered by the fields
 * described above.
 * On error returns -1.
 */
int pm_settings_set_extra_data(pm_settings, void *extra_data);

/**
 * Get functions for the fields described above.
 * If NULL is given, a default value is returned for the field.
 */
int pm_settings_get_grid_size(pm_settings);
double pm_settings_get_Smax(pm_settings);
double pm_settings_get_tol(pm_settings);
double pm_settings_get_abstol(pm_settings);
int pm_settings_get_iv_max_it(pm_settings);
double pm_settings_get_iv_eps(pm_settings);
/**
 * EXCEPTION: Implied volatility initial guesses on error
 *  will return a NULL and set the the default values in the pointer given.
 */
double *pm_settings_get_iv_init(pm_settings, double *init);
void *pm_settings_get_extra_data(pm_settings);


#ifdef __cplusplus
}
#endif

#endif /* __PM_SETTINGS_H__ */
