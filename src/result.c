#include <stdlib.h>
#include "result.h"

result new_result() {
  result r = (result) malloc(sizeof(double));
  if (r)
    *r = 0;
  return r;
}

void delete_result(result r) {
  free(r);
}

void set_result(result r, double val) {
  if (r)
    *r = val;
}
