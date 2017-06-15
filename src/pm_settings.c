#include <stdlib.h>

#include "pm_settings.h"

struct pm_settings_ {
  int N;
  double Smax;
  double tol;
  double abstol;
  /* impl vol */
  int max_it;
  double eps;
  double init[2];
};

pm_settings new_pm_settings() {
  pm_settings pms = (pm_settings) malloc(sizeof(struct pm_settings_));
  if (pms) {
    pms->N = 50;
    pms->Smax = -1;
    pms->tol = 1e-09;
    pms->abstol = 1e-12;

    pms->max_it = 5;
    pms->eps = 1e-4;
    pms->init[0] = 0.25;
    pms->init[1] = 0.75;
  }
  return pms;
}

void delete_pm_settings(pm_settings pms) {
  free(pms);
}

int pm_settings_set_N(pm_settings pms, int N) {
  if (!pms)
    return -1;
  pms->N = N;
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

int pm_settings_get_N(pm_settings pms) {
  return pms->N;
}

double pm_settings_get_Smax(pm_settings pms) {
  return pms->Smax;
}

double pm_settings_get_tol(pm_settings pms) {
  return pms->tol;
}

double pm_settings_get_abstol(pm_settings pms) {
  return pms->abstol;
}

int pm_settings_get_iv_max_it(pm_settings pms) {
  return pms->max_it;
}

double pm_settings_get_iv_eps(pm_settings pms) {
  return pms->eps;
}

double *pm_settings_get_iv_init(pm_settings pms) {
  return pms->init;
}
