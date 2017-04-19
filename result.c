#include <stdlib.h>

#include "result.h"

result new_result() {
  return malloc(sizeof(double));
}
