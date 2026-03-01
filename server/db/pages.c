#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#include "../model/message.h"

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

  if (fptr == NULL)
    empty_page(page);
  else
    fread(page->contents, sizeof(uint64_t), PAGE_SIZE / sizeof(uint64_t), fptr);

  /*
  const int page_len = PAGE_SIZE / INT64_T_SIZE;
  for (int i = 0; i < page_len; i++) {
    printf("%d: ", i * 64);
    // printBits(sizeof(uint64_t), &page->contents[i]);
  }
  */
      
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

typedef struct {
  int page_index;
  int bit_index;
} PageCord;

PageCord* compute_page_cord_from_num(int num)
{
  PageCord pageCord = {
    floor(num / (sizeof(uint64_t) * 8)),
    page_num % (sizeof(uint64_t) * 8)
  };
  
  return &pageCord;
}

bool has_num_been_seen_in_page(Page* page, int page_num)
{
  int page_index = floor(page_num / (sizeof(uint64_t) * 8)); //size of a uint64_t and its 8 bits
  int bit_index = page_num % (sizeof(uint64_t) * 8);

  // gets the specific bitset
  // extracts the specific bit in the bitset
  // takes it out to be compared
  return ((1 << bit_index) & page->contents[page_index]) >> bit_index == 1;
}

void flip_bit_in_page(Page* page, int num)
{
  int page_index = floor(num / (sizeof(uint64_t) * 8)); //size of a uint64_t and its 8 bits
  int bit_index = num % (sizeof(uint64_t) * 8);


  /*
  printf("Flipping bit for %d\n", num);
  printf("Pg Idx: %d\n", page_index);
  printf("Bit Idx: %d\n", bit_index);
  */

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
