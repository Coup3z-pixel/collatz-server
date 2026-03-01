#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "addr_pool.h"

void initialize_addr_pool(AddressPool* addr_pool, const int MAX_SIZE)
{
  addr_pool->map_size = MAX_SIZE;
  addr_pool->map = (int**)calloc(MAX_SIZE, sizeof(int*));;

  IndexQueue* idxQueue = createQueue();

  for (int i = 0; i < MAX_SIZE; i++)
    enqueue(idxQueue, i);

  addr_pool->idxQueue = idxQueue;
}

int insert_addr(AddressPool* addr_pool, char* addr)
{
  int addr_index = dequeue(addr_pool->idxQueue);

  addr_pool->map[addr_index] = malloc(sizeof(addr));
  memcpy(addr_pool->map[addr_index], addr, strlen(addr));

  printf("Added Client Address %ls at %d\n", addr_pool->map[addr_index], addr_index);

  return addr_index;
}

void remove_addr(AddressPool* addr_pool, int addr_index)
{
  addr_pool->map[addr_index] = 0;

  enqueue(addr_pool->idxQueue, addr_index);
}

int* query_addr_from(AddressPool* addr_pool, int addr_index)
{
  return addr_pool->map[addr_index];
}

void print_addr_pool(AddressPool* addr_pool)
{
  int pool_size = sizeof(addr_pool) / sizeof(char*);

  for (int i = 0; i < pool_size; i++) {
    printf("%d: %ls\n", i, addr_pool->map[i]);
  }

}
