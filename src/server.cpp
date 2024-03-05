#include "server.h"

Server::Server(int server_port) : server_port_(server_port), client_count_(0) {
  // Создание сокета
  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_ < 0) {
    std::cerr << "Ошибка при создании сокета!" << std::endl;
    exit(1);
  }

  // Настройка адреса сервера
  server_.sin_family = AF_INET;
  server_.sin_addr.s_addr = INADDR_ANY;
  server_.sin_port = htons(server_port_);
};

void Server::Start() {
  // Привязка сокета к адресу и порту
  if (bind(server_socket_, (struct sockaddr*)&server_, sizeof(server_)) < 0) {
    std::cerr << "Ошибка привязки сокета!" << std::endl;
    exit(1);
  }

  // Прослушивание входящих подключений
  if (listen(server_socket_, 5) < 0) {
    std::cerr << "Ошибка прослушивания сокета!" << std::endl;
    exit(1);
  }
  std::cout << "Сервер запущен." << std::endl;
  std::cout << "Сервер прослушивает порт " << server_port_ << "." << std::endl;

  // Обработка входящих подключений
  while (true) {
    int client = accept(server_socket_, nullptr, nullptr);
    if (client == -1) {
      std::cerr << "Ошибка при подключении клиента!";
      exit(1);
    }
    // Создание нового потока для обработки подключения клиента
    std::thread(&Server::ClientThread, this, client).detach();
  }
}

// Подсчет количества букв в сообщении
std::map<char, size_t> Server::CountLetter(const std::string& message) {
  std::map<char, size_t> count;
  for (char c : message) {
    count[c]++;
  }
  return count;
}

// Метод для обработки подключений клиентов
void Server::ClientThread(int client) {
  clients_.insert(client);
  std::cout << "Клиент номер " << client << " подключился к серверу."
            << std::endl;
  client_count_++;
  char buffer[1024];
  int bytes_received;

  // Получение данных от клиента
  while ((bytes_received = recv(client, buffer, 1024, 0)) > 0) {
    std::string message(buffer, bytes_received);
    if (message.find("=c") != std::string::npos ||
        message.find("=clients") != std::string::npos) {
      // Отправка сообщения с количеством подключений к серверу
      std::string response =
          "Количество подключенных клиентов: " + std::to_string(client_count_) +
          ".";
      send(client, response.c_str(), response.length(), 0);
    } else {
      // Отправка сообщения клиенту с количеством букв
      std::string new_message = "Message " + message + "\n";
      for (auto& entry : CountLetter(message)) {
        new_message += entry.first;
        new_message += "\t";
        new_message += std::to_string(entry.second);
        new_message += "\n";
      }
      send(client, new_message.c_str(), new_message.length(), 0);
    }
  }

  if (bytes_received == 0) {
    std::cout << "Клиент номер " << client << " отключился от сервера."
              << std::endl;
  } else {
    std::cerr << "Ошибка получения данных от клиента номер " << client << "!"
              << std::endl;
  }
  client_count_--;
  clients_.erase(client);
  close(client);
}

void Server::Stop() { close(server_socket_); }
