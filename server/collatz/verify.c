#include <stdbool.h>
#include <stdio.h>

#include "verify.h"

bool is_valid_num(Database* db, unsigned long long num) {
  if (num == 4) return true;

  if (has_num_been_seen(db, num)) return true;

  printf("%lld\n", num);

  bool num_is_valid;

  if (num % 2 == 0) num_is_valid = is_valid_num(db, num / 2); 
  else num_is_valid = is_valid_num(db, 3 * num + 1); 

  if (num_is_valid) flip_bit_in_db(db, num);

  return num_is_valid;
}
