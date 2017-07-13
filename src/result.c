#include <stdio.h>
#include <stdlib.h>
#include "result.h"
#include "result_internal.h"

struct result_ {
  double price;
  double *prices;
  double delta;
  double gamma;
  double theta;
  double rho;
  double vega;
  volatility impl_vol;
  int flag;
};

enum {
  PRICE_FLAG = 1,
  PRICES_FLAG = 2,
  DELTA_FLAG = 4,
  GAMMA_FLAG = 8,
  THETA_FLAG = 16,
  RHO_FLAG = 32,
  VEGA_FLAG = 64,
  IMPL_VOL_FLAG = 128
};

result new_result() {
  result r = (result) malloc(sizeof(struct result_));
  if (r) {
    r->price = 0;
    r->prices = NULL;
    r->delta = 0;
    r->gamma = 0;
    r->theta = 0;
    r->rho = 0;
    r->vega = 0;
    r->impl_vol = 0;
    r->flag = 0;
  }
  return r;
}

void delete_result(result r) {
  if (r && (r->flag & PRICES_FLAG))
    free(r->prices);
  free(r);
}

int result_set_price(result r, double val) {
  if (!r)
    return -1;
  r->flag |= PRICE_FLAG;
  r->price = val;
  return 0;
}

int result_set_prices(result r, double *values) {
  if (!r)
    return -1;
  if (r->flag & PRICES_FLAG)
    free(r->prices);
  r->flag |= PRICES_FLAG;
  r->prices = values;
  return 0;
}

int result_set_delta(result r, double val) {
  if (!r)
    return -1;
  r->flag |= DELTA_FLAG;
  r->delta = val;
  return 0;
}

int result_set_gamma(result r, double val) {
  if (!r)
    return -1;
  r->flag |= GAMMA_FLAG;
  r->gamma = val;
  return 0;
}

int result_set_theta(result r, double val) {
  if (!r)
    return -1;
  r->flag |= THETA_FLAG;
  r->theta = val;
  return 0;
}

int result_set_rho(result r, double val) {
  if (!r)
    return -1;
  r->flag |= RHO_FLAG;
  r->rho = val;
  return 0;
}

int result_set_vega(result r, double val) {
  if (!r)
    return -1;
  r->flag |= VEGA_FLAG;
  r->vega = val;
  return 0;
}

int result_set_impl_vol(result r, volatility v) {
  if (!r)
    return -1;
  r->flag |= IMPL_VOL_FLAG;
  r->impl_vol = v;
  return 0;
}

static inline void _warning(const char *vname) {
  printf("Warning: Accesing unstored value (%s) or NULL result structure\n", vname);
}

double result_get_price(result r) {
  if (!(r && (r->flag & PRICE_FLAG))) {
    _warning("price");
    return 0.0;
  }
  return r->price;
}

double *result_get_prices(result r) {
  if (!(r && (r->flag & PRICES_FLAG))) {
    _warning("prices");
    return NULL;
  }
  return r->prices;
}

double result_get_delta(result r) {
  if (!(r && (r->flag & DELTA_FLAG))) {
    _warning("delta");
    return 0.0;
  }
  return r->delta;
}

double result_get_gamma(result r) {
  if (!(r && (r->flag & GAMMA_FLAG))) {
    _warning("gamma");
    return 0.0;
  }
  return r->gamma;
}

double result_get_theta(result r) {
  if (!(r && (r->flag & THETA_FLAG))) {
    _warning("theta");
    return 0.0;
  }
  return r->theta;
}

double result_get_rho(result r) {
  if (!(r && (r->flag & RHO_FLAG))) {
    _warning("rho");
    return 0.0;
  }
  return r->rho;
}

double result_get_vega(result r) {
  if (!(r && (r->flag & VEGA_FLAG))) {
    _warning("vega");
    return 0.0;
  }
  return r->vega;
}

volatility result_get_impl_vol(result r) {
  if (!(r && (r->flag & IMPL_VOL_FLAG))) {
    _warning("implied volatility");
    return 0.0;
  }
  return r->impl_vol;
}
