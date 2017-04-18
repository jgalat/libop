#include <stdio.h>

#include "option.h"

int main(int argc, char const *argv[]) {
  option eu_call_option = new_option(OPTION_CALL, EU_EXERCISE, 1.0);
  return 0;
}
