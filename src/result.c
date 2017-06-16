#include <stdlib.h>
#include "result.h"
#include "result_internal.h"

struct result_ {
  double v;
  double delta;
  double gamma;
  double theta;
  double rho;
  double vega;
  volatility impl_vol;
};

result new_result() {
  result r = (result) malloc(sizeof(struct result_));
  if (r) {
    r->v = -1;
    r->delta = -1;
    r->gamma = -1;
    r->theta = -1;
    r->rho = -1;
    r->vega = -1;
    r->impl_vol = -1;
  }
  return r;
}

void delete_result(result r) {
  free(r);
}

int result_set_v(result r, double val) {
  if (!r)
    return -1;
  r->v = val;
  return 0;
}

int result_set_delta(result r, double val) {
  if (!r)
    return -1;
  r->delta = val;
  return 0;
}

int result_set_gamma(result r, double val) {
  if (!r)
    return -1;
  r->gamma = val;
  return 0;
}

int result_set_theta(result r, double val) {
  if (!r)
    return -1;
  r->theta = val;
  return 0;
}

int result_set_rho(result r, double val) {
  if (!r)
    return -1;
  r->rho = val;
  return 0;
}

int result_set_vega(result r, double val) {
  if (!r)
    return -1;
  r->vega = val;
  return 0;
}

int result_set_impl_vol(result r, volatility v) {
  if (!r)
    return -1;
  r->impl_vol = v;
  return 0;
}

/* TODO null checks */
double result_get_v(result r) {
  return r->v;
}
double result_get_delta(result r) {
  return r->delta;
}
double result_get_gamma(result r) {
  return r->gamma;
}
double result_get_theta(result r) {
  return r->theta;
}
double result_get_rho(result r) {
  return r->rho;
}
double result_get_vega(result r) {
  return r->vega;
}
volatility result_get_impl_vol(result r) {
  return r->impl_vol;
}
