#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "pages.h"

Page* parse_page_from_file(char* filepath)
{
  Page* page;
  page = malloc(sizeof(Page));

  return page;
}

bool has_num_been_seen(Page* page, int page_num)
{

  int page_index = floor(page_num / sizeof(uint64_t));
  int bit_index = page_num % sizeof(uint64_t);

  // sorry i know its ugly
  // gets the specific bitset
  // extracts the specific bit in the bitset
  // takes it out to be compared
  return ((1 << bit_index) & page->contents[page_index]) >> bit_index == 1;
}

void flip_bit_in_page(Page* page, int page_num)
{
  int page_index = floor(page_num / sizeof(uint64_t));
  int bit_index = page_num % sizeof(uint64_t);

  // sorry i know its ugly
  // gets the specific bitset
  // extracts the specific bit in the bitset
  // takes it out to be compared
  page->contents[page_index] = ((1 << bit_index) | page->contents[page_index]);
}
