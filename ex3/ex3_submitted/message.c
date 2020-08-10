#include "message.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define NO_END_CODE_IN_MESSAGE 0
#define DEAULT_LEN_MSG_NOT_FULLY_RECVED NO_END_CODE_IN_MESSAGE  // as long as end code was not received this is the
// default value of msg->length. this macro has the same value but we wanted to make the code more readable
#define END_CODE "\r\n\r\n"
#define END_CODE_LEN 4
#define BUFFER_SIZE 500

int findEndingCode(const char *buffer, const int buffer_length, const char *ending_code);
int findEndOfMessage(char *buffer, int buffer_length, int end_codes_num);
char *copyBufferToMsg(char *data, int data_length, char *buffer, int buffer_length);
bool isFullMsgRecieved(struct message *msg);

int findEndingCode(const char *buffer, const int buffer_length, const char *ending_code)
{  // returns the index of the ending code. or NO_END_CODE_IN_MESSAGE if not found
  for (int i = 0; i < buffer_length - END_CODE_LEN + 1; i++) {
    bool found_ending_code = true;
    for (int j = 0; j < END_CODE_LEN; j++) {
      if (buffer[i + j] != ending_code[j]) {
        found_ending_code = false;
        break;  // less complex
      }
    }
    if (found_ending_code) {
      return (i + END_CODE_LEN);
    }
  }
  return NO_END_CODE_IN_MESSAGE;
}

int findEndOfMessage(char *buffer, int buffer_length, int end_codes_num)
{  // returns the total length of the message with ending code, or NO_END_CODE_IN_MESSAGE if not found
  int counter, endOfmsg = NO_END_CODE_IN_MESSAGE, totalEndofMsg = 0;
  for (counter = 0; counter < end_codes_num; counter++) {
    endOfmsg = findEndingCode(buffer + endOfmsg, buffer_length - endOfmsg, END_CODE);
    if (endOfmsg == NO_END_CODE_IN_MESSAGE) {
      return NO_END_CODE_IN_MESSAGE;
    }
    totalEndofMsg += endOfmsg;
  }
  return totalEndofMsg;
}

struct message *receiveMessage(int desc, int end_codes_num)
{
  struct message *msg = (struct message *)malloc(sizeof(struct message));
  msg->length_to_send = DEAULT_LEN_MSG_NOT_FULLY_RECVED;
  msg->data = NULL;
  char buffer[BUFFER_SIZE];
  int total_rec_bytes = 0;
  while (!isFullMsgRecieved(msg)) {
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(desc, buffer, BUFFER_SIZE, 0);
    msg->data = copyBufferToMsg(msg->data, total_rec_bytes, buffer, bytes_received);
    total_rec_bytes += bytes_received;
    msg->length_to_send = findEndOfMessage(msg->data, total_rec_bytes, end_codes_num);
  }
  return msg;
}

bool isFullMsgRecieved(struct message *msg) { return msg->length_to_send != DEAULT_LEN_MSG_NOT_FULLY_RECVED; }

char *copyBufferToMsg(char *data, int data_length, char *buffer, int buffer_length)
{
  char *new_data = (char *)malloc(((buffer_length + data_length) * sizeof(char)) + 1);
  if (data != NULL) {
    memcpy(new_data, data, data_length);
    free(data);
  } else {
    data_length = 0;
  }
  memcpy(new_data + data_length, buffer, buffer_length);
  return new_data;
}