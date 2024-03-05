#ifndef SRC_SERVER_
#define SRC_SERVER_

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <thread>

class Server {
 public:
  explicit Server(int server_port);
  ~Server() { Stop(); };
  void Start();
  void ClientHandler(int client);
  void Stop();

 private:
  int server_socket_;
  int server_port_;
  struct sockaddr_in server_;
  size_t client_count_;
  std::map<char, size_t> CountLetter(const std::string &message);
};

#endif  // SRC_SERVER_