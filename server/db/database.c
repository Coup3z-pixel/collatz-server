#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include "database.h"

/*

typedef struct {
  char* filename;
  unsigned long long db_size;
  Page* pages;
} Database;

*/

/*
  * Page Size: 4k
*/


void init_db_conn(Database* db, char* file_name)
{
  db->filename = file_name;
  
  // get collection of pages in filename
  struct dirent *de;  // Pointer for directory entry

  // opendir() returns a pointer of DIR type. 
  DIR *dr = opendir(db->filename);

  if (dr == NULL)  // opendir returns NULL if couldn't open directory
  {
    printf("Could not open current directory" );
    return 0;
  }

  // Refer https://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
  // for readdir()
  while ((de = readdir(dr)) != NULL)
    printf("%s\n", de->d_name);

  closedir(dr); 
  
  db->db_size = 0;

}

bool has_num_been_seen(Database* db, int num)
{
  char* partition_name;

  char* filepath;

  strcpy(filepath, db->filename);
  sprintf(partition_name, "%lld\n", num);
  strcat(filepath, partition_name);

    
  FILE* partition_file = fopen(filepath, "r");

  // get partition in filename
  // find value in partition
  // return if value is 1
  
  return true;
}

void flip_num_in_db(Database* db, int num)
{

}
