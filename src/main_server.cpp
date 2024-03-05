#include "server.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Необходимо ввести порт сервера (например, 8000)!"
              << std::endl;
    exit(1);
  }
  int server_port = std::stoi(argv[1]);
  if (server_port <= 0) {
    std::cerr << "Ошибка ввода!" << std::endl;
    exit(1);
  }

  Server server(server_port);
  server.Start();
  return 0;
}
