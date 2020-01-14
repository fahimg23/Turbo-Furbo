#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <chrono>

#include "Server.h"
#include "Client.h"
#include "Camera_Server.h"

using namespace std;

Camera_Server::Camera_Server(unsigned short port_num,
                              char const* ip_addr,
                              int conn_type,
                              short domain) : Server::Server(port_num, ip_addr, conn_type, domain) {}

int Camera_Server::write_frame(char const* frame_data) {
  int status;

  status = Server::write_data(frame_data);

  if (!status)
    printf("Error sending frame data.\n");

  status = Server::write_data("end");

  if (!status)
    printf("Error sending end of frame message.\n");

  return status;
}
