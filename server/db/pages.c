#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#include "pages.h"

Page* empty_page()
{
  Page* page = malloc(sizeof(Page));

  uint64_t page_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < page_len; i++) {
    page->contents[i] = 0;
  }

  return page;
}

Page* create_page(Page* page, uint64_t interval, char* contents)
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

  fptr = fopen(filepath, "r");

  char* file_content = malloc(PAGE_SIZE);

  if (fptr == NULL) {
    return empty_page();
  } else {
    fgets(file_content, PAGE_SIZE, fptr);
  }

  Page* page = malloc(sizeof(Page));

  return create_page(page, 0, file_content);
}

void save_page_to_file(Page* page, char* filepath)
{
  FILE *fptr;

  fptr = fopen(filepath, "w");

  int content_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < content_len; i++)
  {
    fprintf(fptr, "%" PRIu64 "", page->contents[i]);
  }
  
  fclose(fptr);

  return;
}

bool has_num_been_seen_in_page(Page* page, int page_num)
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

void print_page(Page* page)
{
  uint64_t page_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < page_len; i++) {
    printf("%ld\n", page->contents[i]);
  }
}
