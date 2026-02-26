#include <stdbool.h>
#include <stdio.h>
#include "../db/database.h"


bool is_valid_num(Database* db, unsigned long long num) {
  if (num == 4) { return true; }

  bool num_is_valid;

  if (num % 2 == 0) { 
    num_is_valid = is_valid_num(db, num / 2); 
  } else { 
    num_is_valid = is_valid_num(db, 3 * num + 1); 
  }

  return num_is_valid;
}
