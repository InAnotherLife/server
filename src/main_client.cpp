#include "client.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Необходимо ввести адрес и порт сервера (например, "
                 "\"127.0.0.1\" и 8000)!"
              << std::endl;
    exit(1);
  }

  Client client(argv[1], std::stoi(argv[2]));
  client.Connect();
  return 0;
}
