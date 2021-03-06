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

using namespace std;

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

  char data[MAX_BUFFER];
  memset(data, 'h', MAX_BUFFER*sizeof(char));
  // if udp connection, need to alert server by sending a message
  if (Server::get_conn_type() == SOCK_DGRAM){
    if(!write_data((char const*)data)){
      printf("Could not send first message to server.\n");
      return 0;
    }
  }
  return 1;
}

/* Read data sent from client/server */
int Client::read_data(char* rdata, int& num_bytes_read)
{
  char* read_buf = dbuf_read;

  if(get_conn_type() == SOCK_DGRAM) // UDP
    num_bytes_read = recvfrom(new_socket, read_buf, MAX_BUFFER, 0, (struct sockaddr*)&cli_addr, (socklen_t*)&len_addr);
  else // conn_type == SOCK_STREAM (TCP)
    num_bytes_read = recv(new_socket, read_buf, MAX_BUFFER, 0);

  if (num_bytes_read == -1)
  {
    printf("Error reading data from client.\n");
    return 0;
  }

  for (int i = 0; i < MAX_BUFFER; i++)
    rdata[i] = dbuf_read[i];

  return 1;
}

/* Send data to client/server */
int Client::write_data(char const* wdata)
{
  int status;
  int flags = 0;

  //flags |= MSG_DONTWAIT;

  if (strlen(wdata) > MAX_BUFFER)
  {
    printf("Please limit to %i characters.\n", MAX_BUFFER);
    return 0;
  }

  if(get_conn_type() == SOCK_DGRAM) // UDP
    status = sendto(new_socket, wdata, MAX_BUFFER, flags, (const struct sockaddr*)&cli_addr, (socklen_t)len_addr);
  else // conn_type == SOCK_STREAM (TCP)
    status = send(new_socket, wdata, strlen(wdata), flags);

  if (status == -1)
  {
    printf("Error writing data to client.\n");
    return 0;
  }

  return 1;
}
