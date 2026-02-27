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
  
  // get collection of pages in filename
  struct dirent *de;  // Pointer for directory entry

  // opendir() returns a pointer of DIR type. 
  DIR *dr = opendir(db->filename);

  if (dr == NULL)  // opendir returns NULL if couldn't open directory
  {
    printf("Could not open current directory" );
  }

  int page_count = -2; // -2 for [.., .] in dirs

  while ((de = readdir(dr)) != NULL) {
    page_count++;
  }
    
  closedir(dr); 
}

char* get_page_path(Database* db, int page_count)
{
  int page_path_len = strlen(db->filename) + 1 + get_digit_len(page_count);
  char* page_path;
  page_path = malloc(page_path_len);

  sprintf(page_path, "%s/%d", db->filename, page_count);

  return page_path;
}

void flip_bit_in_db(Database* db, uint64_t num)
{
  // shifting from 1 to 0
  num -= 1;

  int page_count = floor(num / PAGE_SIZE);

  char* page_path = get_page_path(db, page_count);

  Page* num_page = parse_page_from_file(page_path);

  flip_bit_in_page(num_page, num);
  save_page_to_file(num_page, page_path);
}

bool has_num_been_seen(Database* db, uint64_t num)
{
  // shifting from 1 to 0
  num -= 1;
  int page_count = floor(num / PAGE_SIZE);

  char* page_path = get_page_path(db, page_count);

  // get partition in filename
  Page* num_page = parse_page_from_file(page_path);
    
  // find value in partition
  bool seen = has_num_been_seen_in_page(num_page, num);

  return seen;
}
