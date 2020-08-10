#ifndef MESSAGE_H
#define MESSAGE_H

struct message {
  char* data;
  int length_to_send;
};

struct message* receiveMessage(int desc, int end_codes_num);

#endif  // MESSAGE_H
