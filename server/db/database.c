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

typedef struct {
  int page_count;
  int page_num;
} DatabaseCord;

DatabaseCord compute_db_cords(uint64_t num)
{
  DatabaseCord db_cord = {
    floor(num / PAGE_SIZE),
    num % PAGE_SIZE
  };

  return db_cord;
}

Page* get_page_from_num(Database* db, DatabaseCord* db_cord)
{
  char* page_path = get_page_path(db, db_cord->page_count);
  return parse_page_from_file(page_path);
}

void flip_bit_in_db(Database* db, uint64_t num)
{
  DatabaseCord db_cord = compute_db_cords(num - 1);
  Page* num_page = get_page_from_num(db, &db_cord);
  
  flip_bit_in_page(num_page, db_cord.page_num);

  return;
}

bool has_num_been_seen(Database* db, uint64_t num)
{
  DatabaseCord db_cord = compute_db_cords(num - 1);
  Page* num_page = get_page_from_num(db, &db_cord);
      
  bool seen = has_num_been_seen_in_page(num_page, db_cord.page_num);

  return seen;
}
