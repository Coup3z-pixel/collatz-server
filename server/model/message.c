#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "message.h"

void printBits(size_t const size, void const * const ptr)
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
 * Gets the user requested num 
 * after the colon in message
*/
int parse_user_num(char* message)
{
  int colon_pos = get_colon_position(message);

  char* num_str = malloc(strlen(message) - colon_pos);
  strncpy(num_str, message + colon_pos + 1, strlen(message));

  return atoi(num_str);
}

/*
  * Returns the colon position of the user msg
  * @param msg: &char
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
  * @param message: &char
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
  * @param client_address: &char
  * @param payload: &char
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
  * @param Message: &Message
*/
void send_response(Message* message)
{
  write(message->connfd, message->payload, strlen(message->payload));

  return;
}
