#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

class Camera_Server : public Server {
public:
  Camera_Server(unsigned short port_num,
                char const* ip_addr = INADDR_ANY,
                int conn_type = SOCK_STREAM,
                short domain = AF_INET);

  int write_frame(char const* wdata);
};
