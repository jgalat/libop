#include <stdlib.h>

#include "pm_settings.h"

static const int DEFAULT_GRID_SIZE = 100;
static const double DEFAULT_S_MAX = -1;
static const double DEFAULT_TOL = 1e-09;
static const double DEFAULT_ABS_TOL = 1e-12;
static const int DEFAULT_IV_MAX_IT = 5;
static const double DEFAULT_IV_EPS = 1e-4;
static const double DEFAULT_IV_INIT0 = 0.25;
static const double DEFAULT_IV_INIT1 = 0.75;
static void *DEFAULT_EXTRA_DATA = NULL;

struct pm_settings_ {
  int grid_size;
  double Smax;
  double tol;
  double abstol;
  /* impl vol */
  int max_it;
  double eps;
  double init[2];
  /* custom */
  void *extra_data;
};

pm_settings new_pm_settings() {
  pm_settings pms = (pm_settings) malloc(sizeof(struct pm_settings_));
  if (pms) {
    pms->grid_size = DEFAULT_GRID_SIZE;
    pms->Smax = DEFAULT_S_MAX;
    pms->tol = DEFAULT_TOL;
    pms->abstol = DEFAULT_ABS_TOL;

    pms->max_it = DEFAULT_IV_MAX_IT;
    pms->eps = DEFAULT_IV_EPS;
    pms->init[0] = DEFAULT_IV_INIT0;
    pms->init[1] = DEFAULT_IV_INIT1;

    pms->extra_data = DEFAULT_EXTRA_DATA;
  }
  return pms;
}

void delete_pm_settings(pm_settings pms) {
  free(pms);
}

int pm_settings_set_grid_size(pm_settings pms, int N) {
  if (!pms)
    return -1;
  pms->grid_size = N;
  return 0;
}

int pm_settings_set_Smax(pm_settings pms, double smax) {
  if (!pms)
    return -1;
  pms->Smax = smax;
  return 0;
}

int pm_settings_set_tol(pm_settings pms, double tol) {
  if (!pms)
    return -1;
  pms->tol = tol;
  return 0;
}

int pm_settings_set_abstol(pm_settings pms, double abstol) {
  if (!pms)
    return -1;
  pms->abstol = abstol;
  return 0;
}

int pm_settings_set_iv_max_it(pm_settings pms, int maxit) {
  if (!pms)
    return -1;
  pms->max_it = maxit;
  return 0;
}

int pm_settings_set_iv_eps(pm_settings pms, double eps) {
  if (!pms)
    return -1;
  pms->eps = eps;
  return 0;
}

int pm_settings_set_iv_init(pm_settings pms, double x0, double x1) {
  if (!pms)
    return -1;
  pms->init[0] = x0;
  pms->init[1] = x1;
  return 0;
}

int pm_settings_set_extra_data(pm_settings pms, void *extra_data) {
  if (!pms)
    return -1;
  pms->extra_data = extra_data;
  return 0;
}

int pm_settings_get_grid_size(pm_settings pms) {
  if (!pms)
    return DEFAULT_GRID_SIZE;
  return pms->grid_size;
}

double pm_settings_get_Smax(pm_settings pms) {
  if (!pms)
    return DEFAULT_S_MAX;
  return pms->Smax;
}

double pm_settings_get_tol(pm_settings pms) {
  if (!pms)
    return DEFAULT_TOL;
  return pms->tol;
}

double pm_settings_get_abstol(pm_settings pms) {
  if (!pms)
    return DEFAULT_ABS_TOL;
  return pms->abstol;
}

int pm_settings_get_iv_max_it(pm_settings pms) {
  if (!pms)
    return DEFAULT_IV_MAX_IT;
  return pms->max_it;
}

double pm_settings_get_iv_eps(pm_settings pms) {
  if (!pms)
    return DEFAULT_IV_EPS;
  return pms->eps;
}

double *pm_settings_get_iv_init(pm_settings pms, double *init) {
  if (!pms) {
    init[0] = DEFAULT_IV_INIT0;
    init[1] = DEFAULT_IV_INIT1;
    return NULL;
  }
  return pms->init;
}

void *pm_settings_get_extra_data(pm_settings pms) {
  if (!pms)
    return DEFAULT_EXTRA_DATA;
  return pms->extra_data;
}
