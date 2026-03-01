#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "message.h"

/*
 * Returns the length of num
 * @param int num the num which length of digits we want
 * @return the number of digits in num
*/
int get_digit_len(int num) 
{
  if (num == 0) return 1;
  return log10(num);
}

/*
 * Prints the binary given by ptr
 * @param size_t const size: the size of the content inside the ptr
 * @param void const * const ptr: the pointer to the content
*/
void print_bits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

/*
 * Gets the user_num from message format: xxx:user_num
 * @param char* message the message given by the user
*/
int parse_user_num(char* message)
{
  int colon_pos = get_colon_position(message);

  char* num_str = malloc(strlen(message) - colon_pos);
  strncpy(num_str, message + colon_pos + 1, strlen(message));

  return atoi(num_str);
}

/*
  * Returns the colon position of the user msg format cxx:xxxx
  * @param char* msg the message seen in the request
  * @return int the position of the colon in msg
*/
int get_colon_position(char* msg) 
{
  char* colon_ptr = strchr(msg, ':');
  int colon_idx;
  colon_idx = colon_ptr == NULL ? -1 : (int)(colon_ptr - msg);

  return colon_idx;
}

/*
  * Returns the client address or id from user msg
  * @param char* message the message seen in the requests
  * @return char* the sockfd for the client address
*/
char* parse_client_address(char* message)
{
  int colon_idx = get_colon_position(message);

  char* client_address;
  int client_addr_len = colon_idx == -1 ? strlen(message) - 1 : strlen(message) - colon_idx - 1;
  client_address = malloc(client_addr_len);
  strncpy(client_address, message + 1, client_addr_len);
  
  return client_address;
}

/*
  * Creates a Message structs given by params
  * @param int connfd the connection file descriptor of the client
  * @param char* payload the msg being sent back to the client
  * @return Message* the message composed from the params
*/
Message* init_message(int connfd, char* payload)
{
  Message* msg;
  msg = malloc(sizeof(Message));

  msg->connfd = connfd;
  msg->payload = payload;

  return msg;
}

/*
  * Sends message to associated client addr
  * @param Message* message the message which will be used to send information to client
*/
void send_response(Message* message)
{
  write(message->connfd, message->payload, strlen(message->payload));

  return;
}
