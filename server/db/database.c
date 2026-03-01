#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "database.h"
#include "../model/message.h"

#define PAGE_SIZE 4096

/*
  * Creates a connection to the filepath where the db lies in
  * @param Database* db the database struct that will be filled
  * @param char* file_name the address where the database content lies
*/
void initialize_db_conn(Database* db, char* file_name)
{
  db->filename = file_name;
}

/*
  * Returns the path of the page in the database
  * @param Database* db the database struct where the db address lies
  * @param int page_count the page which will be accessed
  * @return the file address to page_count in db
*/
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

/*
  * Returns the database coordinates in db
  * @param uint64_t num the num which will be accessed in db
  * @return DatabaseCord the coordinates to access num in db
*/
DatabaseCord compute_db_cords(uint64_t num)
{
  DatabaseCord db_cord = {
    floor(num / PAGE_SIZE),
    num % PAGE_SIZE
  };

  return db_cord;
}

/*
  * Parses the associated page of db_cord in db
  * @param Database* db the database being accessed
  * @param DatabaseCord* db_cord the position of the num we want to access in db
  * @return Page* the page for the associated db_cord in db
*/
Page* get_page_from_num(Database* db, DatabaseCord* db_cord)
{
  char* page_path = get_page_path(db, db_cord->page_count);
  return parse_page_from_file(page_path);
}

/*
  * Flips in the bit in the db from the associated num
  * @param Database* db the db that is being accessed
  * @param uint64_t num the num that will be flipped to 1
*/
void flip_bit_in_db(Database* db, uint64_t num)
{
  DatabaseCord db_cord = compute_db_cords(num - 1);
  Page* num_page = get_page_from_num(db, &db_cord);
  
  flip_bit_in_page(num_page, db_cord.page_num);

  return;
}

/*
  * Returns if the num has been flipped and hence seen before
  * @param Database* db the db that is being accessed
  * @param uint64_t num the num that we want to see if it has been seen
  * @return bool whether or not the num has been seen
*/
bool has_num_been_seen(Database* db, uint64_t num)
{
  DatabaseCord db_cord = compute_db_cords(num - 1);
  Page* num_page = get_page_from_num(db, &db_cord);
      
  bool seen = has_num_been_seen_in_page(num_page, db_cord.page_num);

  return seen;
}
