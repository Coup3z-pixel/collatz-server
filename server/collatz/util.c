#include <math.h>

#include "util.h"

int get_digit_len(int num) {
  if (num == 0) return 1;
  return log10(num);
}
