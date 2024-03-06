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
  Client(const std::string &address, int port);
  ~Client() { Disconnect(); };

  void Connect();
  void Disconnect();

 private:
  int socket_;
  std::string address_;
  int port_;
  struct sockaddr_in server_;
};

#endif  // SRC_CLIENT_
