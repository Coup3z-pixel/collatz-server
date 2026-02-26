#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INPUT_LEN 32

const int SERVER_ADDR_ARG = 1;
const int CLIENT_ADDR_ARG = 2;

/*
  * creates a fifo and sends it to server address 
  * to allow communication to be established
*/

int initiate_protocol(char* server_address, char* client_address)
{
  printf("Creating Connection to %s\n", server_address);
  
  int server_fd = open(server_address, O_WRONLY);

  mkfifo(client_address, 0666);

  char *append_request_msg = malloc(sizeof(server_address) + 1);;

  strcat(append_request_msg, "a");
  strcat(append_request_msg, client_address);

  // send client_address to add to pool
  if (write(server_fd, append_request_msg, strlen(append_request_msg)+1) == -1) {
    perror("write");
    return 400;
  }
  
  int fd;
  fd = open(client_address, O_RDONLY);
  char buffer[1024];

  printf("Sent %s to %s\n", append_request_msg, server_address);

  // check for allow message
  for(;;) 
  {
    int bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
      buffer[bytes] = '\0';
      printf("Received: %s\n", buffer);
    }
    return strtoull(buffer, NULL, 10);
  }
}

void request_if_num_is_valid(const unsigned long long user_num, const char* server_address, const int CLIENT_ID, const char* client_address) {

  char *append_request_msg = malloc(sizeof(server_address) + 1);;

  char* client_id_str;
  client_id_str = malloc(sizeof(CLIENT_ID));
  sprintf(client_id_str, "%d", CLIENT_ID);

  char* user_num_str;
  user_num_str = malloc(sizeof(user_num));
  sprintf(user_num_str, "%lld", user_num);


  strcat(append_request_msg, "c");
  strcat(append_request_msg, client_id_str);
  strcat(append_request_msg, ":");
  strcat(append_request_msg, user_num_str);

  int server_fd = open(server_address, O_WRONLY);

  if (write(server_fd, append_request_msg, strlen(append_request_msg)+1) == -1) {
    perror("write");
  }

  int fd;
  fd = open(client_address, O_RDONLY);
  char buffer[1024];

  for(;;) {
    int bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
      buffer[bytes] = '\0';
      printf("Received: %s\n", buffer);
    }
    return;
  }

}

void init_repl(const char* server_address, const int CLIENT_ID, char* client_address) {
  for(;;) {
    char buff[MAX_INPUT_LEN];
    int n = MAX_INPUT_LEN;

    unsigned long long user_num;

    while (true) {
      printf("Enter a num: \n");
    
      fgets(buff, n, stdin);
      user_num = strtoull(buff, NULL, 10);

      break;
    }

    printf("You entered: %lld\n", user_num);

    request_if_num_is_valid(user_num, server_address, CLIENT_ID, client_address); 
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "ERROR: MISSING COMMAND LINE ARGUMENTS\n");
    return 1;
  }

  char client_address[128] = "/tmp/";
  char server_address[128] = "/tmp/";

  strcat(client_address, argv[CLIENT_ADDR_ARG]);
  strcat(server_address, argv[SERVER_ADDR_ARG]);

  printf("WELCOME TO THE COLLATZ CONJECTURE LOCAL SERVER PROTOCOL: \n");
  printf("Client: %s\n", client_address);
  printf("Server: %s\n", server_address);

  const int CLIENT_ID = initiate_protocol(server_address, client_address);

  printf("CLIENT_ID: %d\n", CLIENT_ID);

  init_repl(server_address, CLIENT_ID, client_address);

  return EXIT_SUCCESS;
}
