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

Server::Server(unsigned short port_num,
                char const* ip_addr,
                int conn_type,
                short domain)
{
  len_addr = sizeof(addr);
  strcpy(this->ip_addr, ip_addr);
  this->port_num = port_num;
  this->conn_type = conn_type;
  this->domain = domain;
}

/* Accessors and mutators */
unsigned short Server::get_port_num(void) { return port_num; }
void Server::set_port_num(int port_num) { this->port_num = port_num; }

void Server::get_ip_addr(char* ip_addr)
{
  strcpy(ip_addr, this->ip_addr);
}
void Server::set_ip_addr(char const * ip_addr)
{
  strcpy(this->ip_addr, ip_addr);
}

int Server::get_conn_type(void) { return conn_type; }
void Server::set_conn_type(int conn_type) { this->conn_type = conn_type; }

short Server::get_domain(void) { return domain; }
void Server::set_domain(short domain) { this->domain = domain; }

/* Iniitialize socket */
int Server::initialize(void)
{
  // Create socket file descriptor
   if ((server_fd = socket(domain, conn_type, 0)) == 0)
   {
       printf("Could not get socket file descriptor.\n");
       return 0;
   }

   // Attach socket to ip and port
   addr.sin_family = domain;
   inet_pton(domain, ip_addr, &addr.sin_addr); // sets ip address of socket
   addr.sin_port = htons(port_num); // sets port for socket

   if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
   {
     printf("Could not bind socket to ip/port.\n");
     return 0;
   }

   return 1;
}

/* Listen for client connection requests */
int Server::listen_conn(int max_queue)
{
  if (listen(server_fd, max_queue) < 0)
  {
    printf("Error listening for client connection request.\n");
    return 0;
  }

  return 1;
}

/* Accept connection */
int Server::accept_conn(void)
{
  new_socket = accept(server_fd, (struct sockaddr*)&addr, (socklen_t*)&len_addr);
  if (new_socket < 0)
  {
    printf("Error accepting connection.\n");
    return 0;
  }

  return 1;
}

/* Read data sent from client/server */
int Server::read_data(char* rdata)
{
  int num_bytes_read;
  int total_bytes_recv;

  char* read_buf = dbuf_read;

  total_bytes_recv = 0;

  num_bytes_read = recv(new_socket, read_buf, MAX_BUFFER, 0);
  total_bytes_recv += num_bytes_read;

  while(total_bytes_recv < MAX_BUFFER && num_bytes_read > 0) {
    read_buf += num_bytes_read;
    num_bytes_read = recv(new_socket, read_buf, MAX_BUFFER, 0);
    total_bytes_recv += num_bytes_read;
  }

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
int Server::write_data(char const* wdata)
{
  int status;
  int flags = 0;

  //flags |= MSG_DONTWAIT;

  if (strlen(wdata) >= MAX_BUFFER)
  {
    printf("Please limit to %i characters.\n", MAX_BUFFER);
    return 0;
  }

  status = send(new_socket, wdata, strlen(wdata), flags);

  if (status == -1)
  {
    printf("Error writing data to client.\n");
    return 0;
  }

  return 1;
}

int Server::read_data_udp(char* rdata){
  int num_bytes_read;
  int total_bytes_recv;

  char* read_buf = dbuf_read;

  total_bytes_recv = 0;

  num_bytes_read = recvfrom(server_fd, read_buf, MAX_BUFFER, 0, (struct sockaddr*)&cli_addr, (socklen_t*)&len_addr);
  total_bytes_recv += num_bytes_read;

  while(total_bytes_recv < MAX_BUFFER && num_bytes_read > 0) {
    read_buf += num_bytes_read;
    num_bytes_read = recvfrom(new_socket, read_buf, MAX_BUFFER, 0, (struct sockaddr*)&cli_addr, (socklen_t*)&len_addr);
    total_bytes_recv += num_bytes_read;
  }

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
int Server::write_data_udp(char const* wdata)
{
  int status;
  int flags = 0;

  //flags |= MSG_DONTWAIT;

  if (strlen(wdata) >= MAX_BUFFER)
  {
    printf("Please limit to %i characters.\n", MAX_BUFFER);
    return 0;
  }

  status = sendto(server_fd, wdata, MAX_BUFFER, flags, (const struct sockaddr*)&cli_addr, (socklen_t)len_addr);

  if (status == -1)
  {
    printf("Error writing data to client.\n");
    return 0;
  }

  return 1;
}

int Server::close_current_conn()
{
  shutdown(new_socket, SHUT_RDWR);
  return close(new_socket);
}

int Server::terminate()
{
  shutdown(server_fd, SHUT_RDWR);
  return close(server_fd);
}
