#include <stdlib.h>

#include "pm_options.h"

struct pm_options_ {
  int N;
  double tol;
  double abstol;
};

pm_options new_pm_options() {
  pm_options pmo = (pm_options) malloc(sizeof(struct pm_options_));
  if (pmo) {
    pmo->N = 50;
    pmo->tol = 1e-09;
    pmo->abstol = 1e-12;
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
