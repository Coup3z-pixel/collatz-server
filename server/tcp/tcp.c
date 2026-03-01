#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sockaddr_in SA;

void setup_sockets(int* socket_fd, struct sockaddr_in servaddr, const int PORT)
{

  // socket create and verification 
  *socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
  if (*socket_fd == -1) { 
    printf("socket creation failed...\n"); 
    exit(0); 
  } 
  else
    printf("Socket successfully created..\n"); 

  bzero(&servaddr, sizeof(servaddr)); 

  // assign IP, PORT 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  servaddr.sin_port = htons(PORT); 

  // Binding newly created socket to given IP and verification 
  if ((bind(*socket_fd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
    printf("socket bind failed...\n"); 
    exit(0); 
  } 
  else
    printf("Socket successfully binded..\n"); 

  // Now server is ready to listen and verification 
  if ((listen(*socket_fd, 5)) != 0) { 
    printf("Listen failed...\n"); 
    exit(0); 
  } 
  else
    printf("Server listening..\n"); 


}
