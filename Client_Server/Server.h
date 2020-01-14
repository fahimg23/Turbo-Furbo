#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#define IP_ADDR_LEN 14
#define MAX_BUFFER 16384 // 2^16

class Server
{
/* These fields are only for use by class internally */
protected:
  /*
  * server_fd is a file descriptor for a created socket
  * new_socket (for TCP) is a socket in a connected state
  * new_socket (for UDP) is the same as server_fd, but if connect() is called,
  * then the socket will only listen for data from the address specified in connect()
  */
  int server_fd, new_socket;
  struct sockaddr_in addr; // will contain port and ip info --> socket address
  struct sockaddr_in cli_addr;
  char dbuf_read[MAX_BUFFER]; // read data buffer
  size_t len_addr;

private:
  unsigned short port_num;
  char ip_addr[IP_ADDR_LEN]; // default to localhost
  int conn_type; // either TCP or UDP, default will be TCP
  short domain; // either IPv4 or IPv6, default will be IPv4

public:
  Server(unsigned short port_num,
        char const* ip_addr = INADDR_ANY,
        int conn_type = SOCK_STREAM,
        short domain = AF_INET);

  unsigned short get_port_num(void);
  void set_port_num(int port_num);

  void get_ip_addr(char* ip_addr);
  void set_ip_addr(char const* ip_addr);

  int get_conn_type(void);
  void set_conn_type(int conn_type);

  short get_domain(void);
  void set_domain(short domain);

  int initialize(void);

  int listen_conn(int max_queue); // max_queue = maximum number of pending connection requests
  int accept_conn(void);
  int close_current_conn();

  virtual int read_data(char* rdata, int& num_bytes_read);
  virtual int write_data(char const* wdata);

  int terminate();
};
