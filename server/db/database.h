#include "partition.h"

typedef struct {
  char* filename;
  unsigned long long db_size;
  Page* pages;
} Database;
