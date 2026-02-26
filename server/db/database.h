#include <stdint.h>

#include "pages.h"

typedef struct {
  char* filename;
  unsigned long long db_size;
  Page* pages;
} Database;

void initialize_db_conn(Database* db, char* file_name);
void flip_num_in_db(Database* db, int num);
bool num_has_been_seen(Database* db, uint64_t num);
