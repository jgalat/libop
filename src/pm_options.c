#include <stdlib.h>

#include "pm_options.h"

struct pm_options_ {
  int N;
  double tol;
  double abstol;
  /* impl vol */
  int max_it;
  double eps;
  double init[2];
};

pm_options new_pm_options() {
  pm_options pmo = (pm_options) malloc(sizeof(struct pm_options_));
  if (pmo) {
    pmo->N = 50;
    pmo->tol = 1e-09;
    pmo->abstol = 1e-12;

    pmo->max_it = 5;
    pmo->eps = 1e-4;
    pmo->init[0] = 0.25;
    pmo->init[1] = 0.75;
  }
  return pmo;
}

void delete_pm_options(pm_options pmo) {
  free(pmo);
}

int pm_options_set_N(pm_options pmo, int N) {
  if (!pmo)
    return -1;
  pmo->N = N;
  return 0;
}
/* TODO NULL CHECK */
int pm_options_get_N(pm_options pmo) {
  return pmo->N;
}

int pm_options_set_tol(pm_options pmo, double tol) {
  if (!pmo)
    return -1;
  pmo->tol = tol;
  return 0;
}

double pm_options_get_tol(pm_options pmo) {
  return pmo->tol;
}

int pm_options_set_abstol(pm_options pmo, double abstol) {
  if (!pmo)
    return -1;
  pmo->abstol = abstol;
  return 0;
}

double pm_options_get_abstol(pm_options pmo) {
  return pmo->abstol;
}

int pm_options_set_iv_max_it(pm_options pmo, int maxit) {
  if (!pmo)
    return -1;
  pmo->max_it = maxit;
  return 0;
}

int pm_options_get_iv_max_it(pm_options pmo) {
  return pmo->max_it;
}

int pm_options_set_iv_eps(pm_options pmo, double eps) {
  if (!pmo)
    return -1;
  pmo->eps = eps;
  return 0;
}

double pm_options_get_iv_eps(pm_options pmo) {
  return pmo->eps;
}

int pm_options_set_iv_init(pm_options pmo, double x0, double x1) {
  if (!pmo)
    return -1;
  pmo->init[0] = x0;
  pmo->init[1] = x1;
  return 0;
}

double *pm_options_get_iv_init(pm_options pmo) {
  return pmo->init;
}
