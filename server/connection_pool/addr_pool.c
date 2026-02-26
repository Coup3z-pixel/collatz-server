#include <stdlib.h>
#include <stdio.h>
#include "addr_pool.h"


void initialize_addr_pool(AddressPool* addr_pool, const int MAX_SIZE)
{
  addr_pool->map_size = MAX_SIZE;
  addr_pool->map = (char**)calloc(MAX_SIZE, sizeof(char*));;
}

int insert_addr_into(AddressPool* addr_pool, char* addr)
{
  int addr_index = rand() % addr_pool->map_size;
  addr_pool->map[addr_index] = addr;
  printf("Added Client Address %s at %d\n", addr, addr_index);
  return addr_index;
}

void remove_addr_into(AddressPool* addr_pool, int addr_index)
{
  addr_pool->map[addr_index] = 0;
}

char* query_addr_from(AddressPool* addr_pool, int addr_index)
{
  return addr_pool->map[addr_index];
}

void print_addr_pool(AddressPool* addr_pool)
{

}
