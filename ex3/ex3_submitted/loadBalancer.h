#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <netinet/in.h>

#define NUMBER_OF_SERVERS 3

struct loadBalancer {
  int LB_socket;
  int http_socket;
  struct sockaddr http_addr;
  socklen_t socket_length;  // we are aware of that this is a constant number. though 'Accept' requires an address of
  // this length and we decided to keep it this way.
  int http_desc;
  struct sockaddr_in servers_addr[NUMBER_OF_SERVERS];
  int servers[NUMBER_OF_SERVERS];
};

struct loadBalancer* initLoadBalancer();
void terminateLB(struct loadBalancer* lb);

#endif  // LOADBALANCER_H
