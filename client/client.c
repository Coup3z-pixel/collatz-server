#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <math.h>

#define MAX 32
#define PORT 8080
#define MAX_INPUT_LEN 32

typedef struct sockaddr_in SA;

const int SERVER_ADDR_ARG = 1;
const int CLIENT_ADDR_ARG = 2;

int get_digit_len(int num) {
  if (num == 0) return 1;
  return log10(num);
}

/*
  * creates a fifo and sends it to server address 
  * to allow communication to be established
*/

int initiate_protocol(char* client_address)
{
  printf("Creating Connection from %s\n", client_address);

  int sockfd;
  struct sockaddr_in servaddr;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
      printf("socket creation failed...\n");
      exit(0);
  }
  else
      printf("Socket successfully created..\n");

  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
      != 0) {
      printf("connection with the server failed...\n");
      exit(0);
  }
  else
      printf("connected to the server..\n");

  return sockfd;
}

void request_if_num_is_valid(const unsigned long long user_num, const int sockfd) {

  char* sock_fd_str;
  sock_fd_str = malloc(get_digit_len(sockfd));
  sprintf(sock_fd_str, "%d", sockfd);

  char* user_num_str;
  user_num_str = malloc(sizeof(user_num));
  sprintf(user_num_str, "%lld", user_num);

  char *append_request_msg = malloc(1 + strlen(sock_fd_str) + 1 + strlen(user_num_str));

  strcat(append_request_msg, "c");
  strcat(append_request_msg, sock_fd_str);
  strcat(append_request_msg, ":");
  strcat(append_request_msg, user_num_str);

  write(sockfd, append_request_msg, strlen(append_request_msg));
  free(append_request_msg);

  char buff[1024];

  read(sockfd, buff, sizeof(buff));
  printf("From Server : %s\n", buff);
}

void init_repl(int sockfd) {
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

    request_if_num_is_valid(user_num, sockfd); 
  }

  char buff[MAX_INPUT_LEN];
  int n;

  for (;;) {
      bzero(buff, sizeof(buff));
      printf("Enter a num: ");
      buff[0] = 'c';
      n = 1;
      while ((buff[n++] = getchar()) != '\n')
          ;
      write(sockfd, buff, sizeof(buff));
      bzero(buff, sizeof(buff));
      read(sockfd, buff, sizeof(buff));
      printf("From Server : %s", buff);
      if ((strncmp(buff, "exit", 4)) == 0) {
          printf("Client Exit...\n");
          break;
      }
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

  const int sockfd = initiate_protocol(client_address);

  printf("SOCK_FD: %d\n", sockfd);

  init_repl(sockfd);

  return EXIT_SUCCESS;
}
