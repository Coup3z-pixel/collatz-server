#include <stdint.h>

#define PAGE_SIZE 4096
#define INT64_T_SIZE 64

typedef struct {
  int64_t interval;
  int64_t contents[PAGE_SIZE / INT64_T_SIZE];
} Page;

bool has_num_been_seen(Page* page, int page_num);
Page* parse_page_from_file(char* filepath);
void flip_bit_in_page(Page* page, int page_num);
