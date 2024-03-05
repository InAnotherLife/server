#include "server.h"

int main() {
  std::cout << "Введите порт сервера (например, 8000): ";
  int port;
  std::cin >> port;
  if (std::cin.fail() || port < 0) {
    std::cerr << "Ошибка ввода!" << std::endl;
    exit(1);
  }

  Server server(port);
  server.Start();
  return 0;
}
