#include <stdlib.h>
#include "result.h"

result new_result() {
  result r = malloc(sizeof(double));
  if (r)
    *r = 0;
  return r;
}

void set_result(result r, double val) {
  if (r != NULL)
    *r = val;
}
