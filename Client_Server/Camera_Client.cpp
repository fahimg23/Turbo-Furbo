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
#include "Camera_Client.h"

using namespace std;

Camera_Client::Camera_Client(unsigned short port_num,
                  const char* serv_ip_addr,
                  int conn_type,
                  short domain) : Client::Client(port_num, serv_ip_addr, conn_type, domain) {}

int Camera_Client::read_frame(char* frame){
  int status = 1;

  /*
  * The end of a frame that is sent is specified by the message "end")
  */
  char frame_end[3];
  int num_bytes_read;
  int total_bytes_read;

  double timeout;
  chrono::system_clock::time_point begin;
  chrono::system_clock::time_point now;
  chrono::duration<double> time_elapsed;

  begin = chrono::system_clock::now();

  timeout = 10.0; // 10 seconds

  num_bytes_read = 0;

  while((!strcmp(frame_end, "end")) && (total_bytes_read < MAX_BUFFER)) {
    status = Client::read_data(frame+num_bytes_read, num_bytes_read);
    total_bytes_read += num_bytes_read;

    memcpy(frame_end, frame, 3); // used check if messaged received is "end"

    if(!status)
      break;

    now = chrono::system_clock::now();
    time_elapsed = (now - begin);

    if(time_elapsed.count() > timeout){
      printf("Error: Reading frame timeout, did not recieve end of frame buffer.\n");
      status = 0;
      break;
    }
  }

  return status;
}
