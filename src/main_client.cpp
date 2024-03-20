#include "client.h"

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      throw std::runtime_error(
          "Необходимо ввести ip-адрес и порт сервера (например, \"127.0.0.1\" "
          "8000)!)");
    }
    Client client(argv[1], std::stoi(argv[2]));
    client.Connect();
  } catch (const std::exception &err) {
    std::cerr << "Ошибка: " << err.what() << std::endl;
  }
  return 0;
}
