#include "loadBalancer.h"

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT_LOWER_BOUND 1025
#define PORT_UPPER_BOUND 63999
#define SOCKET_LISTEN_QUEUE 10
#define LOCALHOST "127.0.0.1"
#define OCCUPIED_PORT_NUM 80

int initSocket(char *port_file_name);

struct loadBalancer *initLoadBalancer()
{
  struct loadBalancer *LB = (struct loadBalancer *)malloc(sizeof(struct loadBalancer));
  LB->LB_socket = initSocket("server_port");
  LB->http_socket = initSocket("http_port");
  LB->socket_length = sizeof(struct sockaddr_in);

  int i;
  for (i = 0; i < NUMBER_OF_SERVERS; ++i) {
    LB->servers[i] = accept(LB->LB_socket, (struct sockaddr *)&(LB->servers_addr[i]), &(LB->socket_length));
  }
  return LB;
}

int initSocket(char *port_file_name)
{
  int new_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in service;
  int selected_port, bind_returned_value = 1, flag = 1;
  bool occupied_port = true;
  service.sin_family = AF_INET;
  service.sin_addr.s_addr = inet_addr(LOCALHOST);
  while (bind_returned_value != 0) {
    if (occupied_port) {
      selected_port = OCCUPIED_PORT_NUM;
      occupied_port = false;
    } else {
      selected_port = (rand() % (PORT_UPPER_BOUND + 1 - PORT_LOWER_BOUND)) + PORT_LOWER_BOUND;
      occupied_port = true;
    }
    service.sin_port = htons(selected_port);
    setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    bind_returned_value = bind(new_socket, (struct sockaddr *)&service, sizeof(service));
  }
  listen(new_socket, SOCKET_LISTEN_QUEUE);

  FILE *port_file_desc;
  port_file_desc = fopen(port_file_name, "w");
  fprintf(port_file_desc, "%d", selected_port);
  fclose(port_file_desc);
  return new_socket;
}

void terminateLB(struct loadBalancer *lb)
{
  shutdown(lb->LB_socket, SHUT_RDWR);
  close(lb->LB_socket);
  shutdown(lb->http_socket, SHUT_RDWR);
  close(lb->http_socket);
  free(lb);
}
