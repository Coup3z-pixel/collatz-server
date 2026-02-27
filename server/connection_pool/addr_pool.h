#include "index_queue.h"

#ifndef ADDR_POOL_H
#define ADDR_POOL_H

typedef struct {
  int map_size;
  char** map;
  IndexQueue* idxQueue;
} AddressPool;

void initialize_addr_pool(AddressPool* addr_pool, const int MAX_SIZE);
int insert_addr(AddressPool* addr_pool, char* addr);
void remove_addr(AddressPool* addr_pool, int addr_index);
char* query_addr_from(AddressPool* addr_pool, int addr_index);

#endif // !ADDR_POOL_H
