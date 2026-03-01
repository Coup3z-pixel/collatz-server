#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>

#include "model/message.h"
#include "collatz/verify.h"
#include "collatz/util.h"
#include "connection_pool/addr_pool.h"
#include "tcp/tcp.h"

#define MAX 32
#define ADDR_POOL_SIZE 64
#define PORT 8080

#define SERVER_ADDR_ARG 1
#define DB_STORAGE_PATH_ARG 2

typedef struct sockaddr_in SA;

typedef struct {
  int* socket_fd;
  SA servaddr;
} SocketSettings;

typedef struct {
  char* server_addr;
  AddressPool* addr_pool;
  Database* db_conn;
  SocketSettings* socket_settings;
} Dependencies;

/*
  * Set up Dependencies for the server to run properly
  * @param dependencies: &Dependencies
  * @param server_address: &char
  * @param db_storage_path: &char
*/
void setup_dependencies(
  Dependencies* dependencies,
  char* server_address,
  char* db_storage_path
) {
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

  int* socket_fd; 
  struct sockaddr_in servaddr = { 0 };
  SocketSettings* socket_settings = malloc(sizeof(SocketSettings));;
  
  setup_sockets(socket_fd, servaddr, PORT);
  socket_settings->socket_fd = socket_fd;
  socket_settings->servaddr = servaddr;

  dependencies->socket_settings = socket_settings;
}

/*
 * Computes the request given by the message request
 * using the dependencies given
 * @param dependencies: Dependencies
 * @param request: &char
*/
Message* compute_request(Dependencies* dependencies, int connfd, char* request)
{
  switch (request[0]) {
    case 'c': // compute
      uint64_t user_num = parse_user_num(request);
      bool valid = is_valid_num(dependencies->db_conn, user_num);

      int resp_payload_len = get_digit_len(user_num) + 4 + (valid ? 4 : 5);
      char* response_payload = malloc(resp_payload_len);
      sprintf(response_payload, "%ld is %s", user_num, valid ? "true" : "false");
      printf("%s\n", response_payload);

      return init_message(connfd, response_payload);
  }

  return NULL;
}

/*
  * Starts the server from accepting client requests then
  * computing those requests
*/
void run_server(Dependencies* dependencies) 
{
  int connfd, len;
  struct sockaddr_in cli;

  len = sizeof(cli);
  
  // Accept the data packet from client and verification 
  connfd = accept(*dependencies->socket_settings->socket_fd, (SA*)&cli, &len); 
  if (connfd < 0) { 
      printf("server accept failed...\n"); 
      exit(0); 
  } 
  else
      printf("server accept the client...\n");

  char buff[MAX]; 

  for (;;) { 
    bzero(buff, MAX); 

    // read the message from client and copy it in buffer 
    read(connfd, buff, sizeof(buff)); 

    printf("From client: %s\t \n", buff); 

    Message* msg = compute_request(dependencies, connfd, buff);
    send_response(msg);
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
