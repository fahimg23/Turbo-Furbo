#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

class Client : public Server
{
private:
  struct sockaddr_in serv_addr; // server address

public:
  Client(unsigned short port_num,
          const char* serv_ip_addr,
          int conn_type = SOCK_STREAM,
          short domain = AF_INET);

  int initialize(void);
  int connect_serv(void);

  int read_data_udp(char* rdata);
  int write_data_udp(char const* wdata);
};
