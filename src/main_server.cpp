#include "server.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Необходимо ввести порт сервера (например, 8000)!"
              << std::endl;
    exit(1);
  }

  Server server(std::stoi(argv[1]));
  server.Start();
  return 0;
}
