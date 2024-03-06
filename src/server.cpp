#include "server.h"

Server::Server(int port) : port_(port) {
  // Создание сокета
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    std::cerr << "Ошибка при создании сокета!" << std::endl;
    exit(1);
  }

  // Настройка адреса сервера
  server_.sin_family = AF_INET;
  server_.sin_addr.s_addr = INADDR_ANY;
  server_.sin_port = htons(port_);
};

void Server::Start() {
  // Привязка сокета к адресу и порту
  if (bind(socket_, (struct sockaddr*)&server_, sizeof(server_)) < 0) {
    std::cerr << "Ошибка привязки сокета!" << std::endl;
    exit(1);
  }

  // Прослушивание входящих подключений
  if (listen(socket_, 5) < 0) {
    std::cerr << "Ошибка прослушивания сокета!" << std::endl;
    exit(1);
  }
  std::cout << "Сервер запущен." << std::endl;
  std::cout << "Сервер прослушивает порт " << port_ << "." << std::endl;

  // Обработка входящих подключений
  while (true) {
    int client = accept(socket_, nullptr, nullptr);
    if (client == -1) {
      std::cerr << "Ошибка при подключении клиента!";
      exit(1);
    }
    // Создание нового потока для обработки подключения
    std::thread(&Server::ClientThread, this, client).detach();
  }
}

// Подсчет количества букв в сообщении
std::map<char, size_t> Server::CountLetter(const std::string& message) {
  std::map<char, size_t> count;
  for (char sym : message) {
    if (isalpha(sym)) {
      count[sym]++;
    }
  }
  return count;
}

// Отправка сообщения
void Server::SendMessage(int client, std::string message) {
  if (message.find("=c") != std::string::npos ||
      message.find("=clients") != std::string::npos) {
    // Отправка сообщения с количеством подключений к серверу
    std::string response =
        "Количество подключенных клиентов: " + std::to_string(clients_.size()) +
        ".";
    send(client, response.c_str(), response.length(), 0);
  } else if (message[0] == '=' && isdigit(message[1])) {
    // Отправка сообщения другому клиенту
    size_t space_pos = message.find(' ');
    int other_client = std::stoi(message.substr(1, space_pos - 1));
    message.erase(0, space_pos + 1);
    if (clients_.count(other_client)) {
      // Если клиент отправил сообщение самому себе
      if (client == other_client) {
        std::string new_message =
            "Вы отправили сообщение сами себе: " + message;
        send(client, new_message.c_str(), new_message.length(), 0);
      } else {
        std::string new_message = "Сообщение для клиента номер " +
                                  std::to_string(other_client) + ": " + message;
        send(other_client, new_message.c_str(), new_message.length(), 0);
      }
    } else {
      std::string error_message = "Клиента с номером " +
                                  std::to_string(other_client) +
                                  " не существует!";
      send(client, error_message.c_str(), error_message.length(), 0);
    }
  } else {
    // Отправка сообщения клиенту с подсчетом количества букв
    std::string new_message = "Message " + message + "\n";
    for (auto& item : CountLetter(message)) {
      new_message += item.first;
      new_message += "\t";
      new_message += std::to_string(item.second);
      new_message += "\n";
    }
    send(client, new_message.c_str(), new_message.length(), 0);
  }
}

// Метод для обработки подключений клиентов
void Server::ClientThread(int client) {
  std::cout << "Клиент номер " << client << " подключился к серверу."
            << std::endl;
  clients_.insert(client);
  char buffer[1024];
  int bytes_received;

  // Получение данных от клиента и отправка сообшения клиенту
  while ((bytes_received = recv(client, buffer, 1024, 0)) > 0) {
    std::string message(buffer, bytes_received);
    SendMessage(client, message);
  }

  if (bytes_received == 0) {
    std::cout << "Клиент номер " << client << " отключился от сервера."
              << std::endl;
  } else {
    std::cerr << "Ошибка получения данных от клиента номер " << client << "!"
              << std::endl;
  }
  clients_.erase(client);
  close(client);
}

void Server::Stop() { close(socket_); }
