#include <stdlib.h>
#include "parameter.h"
#include <debug.h>

struct parameter {
  double value;
};

struct parameter *new_parameter(double val) {
  struct parameter *p = (struct parameter *) malloc(sizeof(struct parameter));
  if (p) {
    p->value = val;
  }
  return p;
}

static const char *__PARAMETER_NULL = "Parameter is NULL";

struct parameter *new_parameter_(struct parameter *p) {
  if (p)
    return new_parameter(p->value);
  __DEBUG(__PARAMETER_NULL);
  return NULL;
}

void delete_parameter(struct parameter *p) {
  free(p);
}

int param_set_value(struct parameter *p, double val) {
  if (!p) {
    __DEBUG(__PARAMETER_NULL);
    return -1;
  }
  p->value = val;
  return 0;
}

double param_get_value(struct parameter *p) {
  if (!p) {
    __DEBUG(__PARAMETER_NULL);
    return 0.0;
  }
  return p->value;
}
