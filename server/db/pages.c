#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#include "pages.h"

Page* empty_page(Page* page)
{
  uint64_t page_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < page_len; i++) {
    page->contents[i] = 0;
  }

  return page;
}

Page* parse_page_from_file(char* filepath)
{
  FILE *fptr;
  Page* page = malloc(sizeof(Page));

  fptr = fopen(filepath, "rb");

  if (fptr == NULL) {
    return empty_page(page);
  } else {
    fread(page->contents, sizeof(uint64_t), PAGE_SIZE / sizeof(uint64_t), fptr);
  }
  
  return page;
}

void save_page_to_file(Page* page, char* filepath)
{
  FILE *fptr;

  fptr = fopen(filepath, "wb");

  int content_len = sizeof(page->contents) / sizeof(uint64_t);

  fwrite(page->contents, content_len, 1, fptr);
  
  fclose(fptr);

  return;
}

bool has_num_been_seen_in_page(Page* page, int page_num)
{
  int page_index = floor(page_num / sizeof(uint64_t));
  int bit_index = page_num % sizeof(uint64_t);

  // gets the specific bitset
  // extracts the specific bit in the bitset
  // takes it out to be compared
  return ((1 << bit_index) & page->contents[page_index]) >> bit_index == 1;
}

void flip_bit_in_page(Page* page, int num)
{
  int page_index = floor(num / sizeof(uint64_t));
  int bit_index = num % sizeof(uint64_t);

  printf("Flipping bit for %d\n", num);
  printf("%d\n", page_index);
  printf("%d\n", bit_index);

  // gets the specific bitset
  // extracts the specific bit in the bitset
  // takes it out to be compared
  page->contents[page_index] = ((1 << bit_index) | page->contents[page_index]);
}

void print_page(Page* page)
{
  uint64_t page_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < page_len; i++) {
    printf("%ld\n", page->contents[i]);
  }
}
