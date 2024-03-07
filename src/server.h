#ifndef SRC_SERVER_
#define SRC_SERVER_

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <vector>

class Server {
 public:
  explicit Server(int port);
  ~Server() { Stop(); };

  void Start();
  void Stop();

 private:
  int socket_;
  int port_;
  struct sockaddr_in server_;
  std::set<int> clients_;
  std::vector<std::thread> client_thread_;
  void ClientThread(int client);
  std::map<char, size_t> CountLetter(const std::string &message);
  void SendMessage(int client, std::string message);
};

#endif  // SRC_SERVER_
