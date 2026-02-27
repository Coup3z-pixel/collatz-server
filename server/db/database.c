#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "database.h"

#define PAGE_SIZE 4096

/*

typedef struct {
  char* filename;
  unsigned long long db_size;
} Database;

*/

/*
  * Page Size: 4k
*/


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
  
  db->db_size = page_count * PAGE_SIZE;
}

bool has_num_been_seen(Database* db, uint64_t num)
{
  int offset = num % PAGE_SIZE;
  int page_count = floor(num / PAGE_SIZE);

  printf("%d\n", offset);
  printf("%d\n", page_count);

  char* page_path;
    int page_path_len = strlen(db->filename) + 1 + log10(page_count) + 10;
  page_path = malloc(page_path_len);

  

  sprintf(page_path, "%s/%d", db->filename, page_count);
  

  // get partition in filename
  Page* num_page = parse_page_from_file(page_path);
    
  // find value in partition
  bool seen = has_num_been_seen_in_page(num_page, offset);

  
  if (!seen) flip_bit_in_page(num_page, offset);
  
  save_page_to_file(page_path, num_page);

  return true;
}
