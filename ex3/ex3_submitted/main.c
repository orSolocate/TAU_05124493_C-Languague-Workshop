#include "loadBalancer.h"
#include "message.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>

#define STATUS_CODE_SUCCESS 1
#define NUM_END_CODES_HTTP_RECV 1
#define NUM_END_CODES_SERVERS_RECV 2
#define NUM_ITERATIONS_VALGRIND_TEST 9
#define VALGRIND_TEST 0  // 1 for YES 0 for NO

int main()
{
  srand(time(0));
  struct loadBalancer* LB = initLoadBalancer();
  int server_ID = 0, i = 0;
  struct message* http_message;
  struct message* server_message;

  while ((VALGRIND_TEST && i < NUM_ITERATIONS_VALGRIND_TEST) || (!VALGRIND_TEST)) {
    LB->http_desc = accept(LB->http_socket, (struct sockaddr*)&(LB->http_addr), &(LB->socket_length));
    http_message = receiveMessage(LB->http_desc, NUM_END_CODES_HTTP_RECV);
    int bytes_sent = 0;
    while (bytes_sent < http_message->length_to_send) {
      bytes_sent +=
          send(LB->servers[server_ID], http_message->data + bytes_sent, (http_message->length_to_send) - bytes_sent, 0);
    }
    free(http_message->data);
    free(http_message);
    server_message = receiveMessage(LB->servers[server_ID], NUM_END_CODES_SERVERS_RECV);

    bytes_sent = 0;
    while (bytes_sent < server_message->length_to_send) {
      bytes_sent +=
          send(LB->http_desc, (server_message->data) + bytes_sent, (server_message->length_to_send) - bytes_sent, 0);
    }
    free(server_message->data);
    free(server_message);
    // prepare for next request
    server_ID = (server_ID + 1) % NUMBER_OF_SERVERS;
    i += 1;
  }
  terminateLB(LB);
  return STATUS_CODE_SUCCESS;
}
