#include "server.h"

int main(int argc, char *argv[]) {
  try {
    if (argc != 2) {
      throw std::runtime_error(
          "Необходимо ввести порт сервера (например, 8000)!");
    }
    int server_port = std::stoi(argv[1]);
    if (server_port <= 0) {
      throw std::runtime_error("Ошибка ввода!");
    }
    Server server(server_port);
    server.Start();
  } catch (const std::exception &err) {
    std::cerr << "Ошибка: " << err.what() << std::endl;
  }
  return 0;
}
