#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Server.h"
#include "Client.h"

Client::Client(unsigned short port_num,
                const char* serv_ip_addr,
                int conn_type,
                short domain) : Server::Server(port_num, serv_ip_addr, conn_type, domain) {}

int Client::initialize(void)
{
  unsigned short port_num;
  int conn_type;
  short domain;
  char serv_ip_addr[IP_ADDR_LEN];

  port_num = Server::get_port_num();
  conn_type = Server::get_conn_type();
  domain = Server::get_domain();
  Server::get_ip_addr(serv_ip_addr);

  if ((new_socket = socket(domain, conn_type, 0)) < 0)
    {
      printf("Could not get socket file descriptor.\n");
      return 0;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = domain;
    serv_addr.sin_port = htons(port_num);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(domain, serv_ip_addr, &serv_addr.sin_addr) <= 0)
    {
        printf("Bad server address.\n");
        return 0;
    }

    return 1;
}

int Client::connect_serv(void)
{
  if(connect(new_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
  {
    printf("Could not connect to server.\n");
    return 0;
  }

  return 1;
}