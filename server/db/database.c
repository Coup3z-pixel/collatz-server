#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "database.h"
#include "../collatz/util.c"

#define PAGE_SIZE 4096

void initialize_db_conn(Database* db, char* file_name)
{
  db->filename = file_name;
}

char* get_page_path(Database* db, int page_count)
{
  int page_path_len = strlen(db->filename) + 1 + get_digit_len(page_count);
  char* page_path = malloc(page_path_len);

  sprintf(page_path, "%s/%d", db->filename, page_count);

  return page_path;
}

Page* get_page_from_num(uint64_t num)
{
  // shifting from 1 to 0
  num -= 1;

  int page_count = floor(num / PAGE_SIZE);
  int page_num = num % PAGE_SIZE;

  char* page_path = get_page_path(db, page_count);
  return parse_page_from_file(page_path);
}

void flip_bit_in_db(Database* db, uint64_t num)
{
  Page* num_page = get_page_from_num(num);
  
  flip_bit_in_page(num_page, page_num);
  save_page_to_file(num_page, page_path);
}

bool has_num_been_seen(Database* db, uint64_t num)
{
  Page* num_page = get_page_from_num(num);
      
  // find value in partition
  bool seen = has_num_been_seen_in_page(num_page, page_num);

  return seen;
}
