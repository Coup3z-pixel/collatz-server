#include <stdint.h>

#include "pages.h"

typedef struct {
  char* filename;
  Page* pages;
} Database;

void initialize_db_conn(Database* db, char* file_name);
bool has_num_been_seen(Database* db, uint64_t num);
void flip_bit_in_db(Database* db, uint64_t num);
