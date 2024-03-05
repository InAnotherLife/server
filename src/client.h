#ifndef SRC_CLIENT_
#define SRC_CLIENT_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

class Client {
 public:
  Client(const std::string &server_address, int server_por);
  ~Client() { Disconnect(); };

  void Connect();
  void Disconnect();

 private:
  int client_socket_;
  std::string server_address_;
  int server_port_;
  struct sockaddr_in server_;
};

#endif  // SRC_CLIENT_
