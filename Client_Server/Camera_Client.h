#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

class Camera_Client : public Client {
public:
  Camera_Client(unsigned short port_num,
                const char* serv_ip_addr,
                int conn_type = SOCK_STREAM,
                short domain = AF_INET);

  int read_frame(char* frame);
};
