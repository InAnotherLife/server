#include "server.h"

Server::Server() : server_port_(8000), client_count_(0) {
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

// Получение порта сервера
void Server::GetPort() {
  std::cout << "Введите порт сервера (например, 8000): ";
  std::cin >> server_port_;
  if (std::cin.fail() || server_port_ < 0) {
    std::cerr << "Ошибка ввода!" << std::endl;
    exit(1);
  }
}

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

  // Цикл обработки входящих подключений
  while (true) {
    // Подключение нового клиента
    int client = accept(server_socket_, nullptr, nullptr);
    if (client == -1) {
      std::cerr << "Ошибка при подключении клиента!";
      exit(1);
    }
    // Создание нового потока для обработки подключения клиента
    std::thread(&Server::ClientHandler, this, client).detach();
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
void Server::ClientHandler(int client) {
  std::cout << "Клиент подключился к серверу." << std::endl;
  client_count_++;
  char buffer[1024];
  int bytes_received;

  // Получение данных от клиента
  while ((bytes_received = recv(client, buffer, 1024, 0)) > 0) {
    std::string message(buffer, bytes_received);
    if (message == "=c" || message == "=clients") {
      // Отправка сообщения с количеством подключений к серверу
      std::string response =
          "Количество подключенных клиентов: " + std::to_string(client_count_) +
          ".";
      send(client, response.c_str(), response.length(), 0);
    } else {
      // Отправка сообщения с количеством букв
      std::string response = "Message " + message + "\n";
      for (auto& entry : CountLetter(message)) {
        response += entry.first;
        response += "\t";
        response += std::to_string(entry.second);
        response += "\n";
      }
      send(client, response.c_str(), response.length(), 0);
    }
  }

  if (bytes_received == 0) {
    std::cout << "Клиент отключился от сервера." << std::endl;
  } else {
    std::cerr << "Ошибка получения данных от клиента!" << std::endl;
  }
  client_count_--;
  close(client);
}

void Server::Stop() { close(server_socket_); }
