#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "collatz/verify.h"
#include "connection_pool/addr_pool.h"

#define MAX_INPUT_LEN 32
#define ADDR_POOL_SIZE 64

const int SERVER_ADDR_ARG = 1;
const int DB_STORAGE_PATH_ARG = 2;

typedef struct {
  char* server_addr;
  AddressPool* addr_pool;
  Database* db_conn;
} Dependencies;

typedef struct {
  char* client_address;
  char* payload;
} Message;

/*
  * creates a fifo and sends it to server address 
  * to allow communication to be established
*/
void setup_dependencies(Dependencies* dependencies, char* server_address, char* db_storage_path)
{
  printf("Creating Dependencies for %s\n", server_address);

  dependencies->server_addr = server_address;

  printf("Server Configurations: \n");
  printf("- Server Address: %s\n", dependencies->server_addr);

  AddressPool* addr_pool = malloc(sizeof(AddressPool));
  initialize_addr_pool(addr_pool, ADDR_POOL_SIZE);
  dependencies->addr_pool = addr_pool;

  printf("- Address Pool Size: %d\n", ADDR_POOL_SIZE);

  Database* db_conn = malloc(sizeof(Database));
  initialize_db_conn(db_conn, db_storage_path);
  dependencies->db_conn = db_conn;

  printf("- Database Connection: %s\n", db_storage_path);
  printf("\n");
}

int get_colon_position(char* msg) 
{
  char* colon_ptr = strchr(msg, ':');
  int colon_idx;
  colon_idx = colon_ptr == NULL ? -1 : (int)(colon_ptr - msg);

  return colon_idx;
}

char* parse_client_address(char* message)
{
  int colon_idx = get_colon_position(message);

  char* client_address;
  int client_addr_len = colon_idx == -1 ? strlen(message) - 1 : strlen(message) - colon_idx - 1;
  client_address = malloc(client_addr_len);
  strncpy(client_address, message + 1, client_addr_len);
  
  return client_address;
}

unsigned long long parse_user_num(char* message)
{
  int colon_idx = get_colon_position(message);

  char* num_str;
  int num_len = strlen(message) - colon_idx - 1;
  num_str = malloc(num_len);

  strncpy(num_str, message + colon_idx + 1, num_len);

  return atoi(num_str);
}

Message* init_message(char* client_address, char* payload)
{
  Message* msg;
  msg = malloc(sizeof(Message));

  msg->client_address = client_address;
  msg->payload = payload;

  return msg;
}


Message* compute_request(Dependencies* dependencies, char* request)
{
  char* client_address = parse_client_address(request);

  switch (request[0]) {
    case 'a':
      const int CLIENT_ID = insert_addr(dependencies->addr_pool, client_address);

      char* client_payload;
      client_payload = malloc(log10(CLIENT_ID));
      sprintf(client_payload, "%d", CLIENT_ID);

      return init_message(client_address, client_payload);

    case 'c':
      const int CLIENT_INDEX = atoi(client_address);
      char* real_client_addr = query_addr_from(dependencies->addr_pool, CLIENT_INDEX);

      uint64_t user_num = parse_user_num(request);

      printf("Processing %ld for %s\n", user_num, real_client_addr);

      bool valid = is_valid_num(dependencies->db_conn, user_num);

      printf("%ld is %s\n", user_num, valid ? "true" : "false");

      char* response_payload;
      int resp_payload_len = 0;

      resp_payload_len += log10(user_num) + 4; // length of num
      resp_payload_len += 4; // length of " is "
      resp_payload_len += valid ? 4 : 5; // length for "true" and "false" resp.

      response_payload = malloc(resp_payload_len);

      sprintf(response_payload, "%ld", user_num);
      strcat(response_payload, " is ");
      strcat(response_payload, valid ? "true" : "false");

      return init_message(real_client_addr, response_payload);
  }

  return NULL;
}

void send_response(Message* message)
{
  int client_fd = open(message->client_address, O_WRONLY);

  if (write(client_fd, message->payload, strlen(message->payload)) == -1) {
    perror("write");
  }

  return;
}

void run_server(Dependencies* dependencies) 
{

  mkfifo(dependencies->server_addr, 0666);

  int server_fd;
  server_fd = open(dependencies->server_addr, O_RDONLY);

  char buffer[1024];

  printf("Server has Started at: %s\n", dependencies->server_addr);

  while (true) {
    for(;;) {
      int bytes = read(server_fd, buffer, sizeof(buffer) - 1);

      if (bytes < 0) continue; // continue if not valid byte
      
      buffer[bytes] = '\0';
      printf("Received: %s\n", buffer);


      Message* response = compute_request(dependencies, buffer);
      send_response(response);
    }
  }
  
  return;
}

int main(int argc, char *argv[])
{
  char server_address[128] = "/tmp/";
  char db_storage_path[128] = "./";

  strcat(server_address, argv[SERVER_ADDR_ARG]);
  strcat(db_storage_path, argv[DB_STORAGE_PATH_ARG]);

  printf("WELCOME TO THE COLLATZ CONJECTURE LOCAL SERVER PROTOCOL: \n");
  printf("Server: %s\n", server_address);

  Dependencies dependencies = { 0 };
  setup_dependencies(&dependencies, server_address, db_storage_path);

  run_server(&dependencies);

  return EXIT_SUCCESS;
}
