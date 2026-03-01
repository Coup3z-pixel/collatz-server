#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

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

  if (fptr == NULL) empty_page(page);
  else fread(page->contents, sizeof(uint64_t), PAGE_SIZE / sizeof(uint64_t), fptr);

  page->filepath = malloc(strlen(filepath));
  strcpy(page->filepath, filepath);
      
  return page;
}

void save_page_to_file(Page* page)
{
  FILE *fptr;

  fptr = fopen(page->filepath, "wb");

  int content_len = sizeof(page->contents) / sizeof(uint64_t);

  fwrite(page->contents, content_len, 1, fptr);
  
  fclose(fptr);

  return;
}

typedef struct {
  int page_index;
  int bit_index;
} PageCord;

PageCord compute_page_cord_from_num(int num)
{
  PageCord pageCord = {
    floor(num / (sizeof(uint64_t) * 8)),
    num % (sizeof(uint64_t) * 8)
  };
  
  return pageCord;
}

bool has_num_been_seen_in_page(Page* page, int page_num)
{
  PageCord pageCord = compute_page_cord_from_num(page_num);

  return ((1UL << pageCord.bit_index) \
    & page->contents[pageCord.page_index]) >> pageCord.bit_index;
}

void flip_bit_in_page(Page* page, int page_num)
{
  PageCord pageCord = compute_page_cord_from_num(page_num);

  printf("Pg Idx: %d\n", pageCord.page_index);
  printf("Bit Idx: %d\n", pageCord.bit_index);

  page->contents[pageCord.page_index] |= 1UL << pageCord.bit_index;

  save_page_to_file(page);
}

void print_page(Page* page)
{
  uint64_t page_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < page_len; i++) {
    printBits(sizeof(page->contents[i]), &page->contents[i]);
  }
}
