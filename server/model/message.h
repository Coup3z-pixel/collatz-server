

typedef struct {
  int connfd;
  char* payload;
} Message;

int get_digit_len(int num);
void print_bits(size_t const size, void const * const ptr);
int get_colon_position(char* msg);
char* parse_client_address(char* message);
int parse_user_num(char* message);
Message* init_message(int connfd, char* payload);
void send_response(Message* message);
