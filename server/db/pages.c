#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "../model/message.h"

#include "pages.h"

/*
* Returns an empty page if the file doesn't exist or fills a page with content
* from the saved file
* @param char* filepath
* @return Page* the page struct that has been parsed from filepath
*/
Page* parse_page_from_file(char* filepath)
{
  FILE *fptr;
  Page* page = malloc(sizeof(Page));

  fptr = fopen(filepath, "rb");

  if (fptr == NULL) ; // if null no need to fill the already empty page
  else fread(page->contents, sizeof(uint64_t), PAGE_SIZE / sizeof(uint64_t), fptr);

  page->filepath = malloc(strlen(filepath));
  strcpy(page->filepath, filepath);
      
  return page;
}

/*
  * Writes to the page content to page.filepath
  * @param Page* page the page being written
*/
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

/*
  * Computes the cordinates of num in the page
  * @param int num the number being converted to its cordinates
  * @return PageCord the cordinates of the number in the page
*/
PageCord compute_page_cord_from_num(int num)
{
  PageCord pageCord = {
    floor(num / (sizeof(uint64_t) * 8)),
    num % (sizeof(uint64_t) * 8)
  };
  
  return pageCord;
}

/*
  * Returns if the number has been seen before
  * @param Page* page the page where the number is contained
  * @param int page_num the num being tested for if its been checked
  * @return bool whether the num has been seen in the page before
*/
bool has_num_been_seen_in_page(Page* page, int page_num)
{
  PageCord pageCord = compute_page_cord_from_num(page_num);

  return ((1UL << pageCord.bit_index) \
    & page->contents[pageCord.page_index]) >> pageCord.bit_index;
}

/*
  * Flips the bit corresponding to page_num in page
  * @param Page* page the page which page_num lies
  * @param int page_num the number in the page being flipped
*/
void flip_bit_in_page(Page* page, int page_num)
{
  PageCord pageCord = compute_page_cord_from_num(page_num);

  printf("Pg Idx: %d\n", pageCord.page_index);
  printf("Bit Idx: %d\n", pageCord.bit_index);

  page->contents[pageCord.page_index] |= 1UL << pageCord.bit_index;

  save_page_to_file(page);
}

/*
  * Prints the page's content in binary
  * @param Page* page the page which the information lies in
*/
void print_page(Page* page)
{
  uint64_t page_len = sizeof(page->contents) / sizeof(uint64_t);

  for (int i = 0; i < page_len; i++) {
    print_bits(sizeof(page->contents[i]), &page->contents[i]);
  }
}
