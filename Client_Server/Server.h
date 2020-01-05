#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#define IP_ADDR_LEN 14
#define MAX_BUFFER 65536 // 2^16

class Server
{
/* These fields are only for use by class internally */
protected:
  int server_fd, new_socket;
  struct sockaddr_in addr; // will contain port and ip info --> socket address
  char dbuf_read[MAX_BUFFER]; // read data buffer
  size_t len_addr;

private:
  unsigned short port_num;
  char ip_addr[IP_ADDR_LEN]; // default to localhost
  int conn_type; // either TCP or UDP, default will be TCP
  short domain; // either IPv4 or IPv6, default will be IPv4

public:
  Server(unsigned short port_num,
        const char* ip_addr = INADDR_ANY,
        int conn_type = SOCK_STREAM,
        short domain = AF_INET);

  unsigned short get_port_num(void);
  void set_port_num(int port_num);

  void get_ip_addr(char* ip_addr);
  void set_ip_addr(const char* ip_addr);

  int get_conn_type(void);
  void set_conn_type(int conn_type);

  short get_domain(void);
  void set_domain(short domain);

  int initialize(void);

  int listen_conn(int max_queue); // max_queue = maximum number of pending connection requests
  int accept_conn(void);
  int close_current_conn();

  int read_data(char* rdata);
  int write_data(const char* wdata);

  int terminate();
};
