#include "server.h"

Server::Server(int port) : port_(port) {
  // Создание сокета
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    throw std::runtime_error("Ошибка при создании сокета!");
  }

  // Настройка адреса сервера
  server_.sin_family = AF_INET;
  server_.sin_addr.s_addr = INADDR_ANY;
  server_.sin_port = htons(port_);
};

void Server::Start() {
  // Привязка сокета к адресу и порту
  if (bind(socket_, (struct sockaddr*)&server_, sizeof(server_)) < 0) {
    throw std::runtime_error("Ошибка привязки сокета!");
  }

  // Прослушивание входящих подключений
  if (listen(socket_, 5) < 0) {
    throw std::runtime_error("Ошибка прослушивания сокета!");
  }
  std::cout << "Сервер запущен." << std::endl;
  std::cout << "Сервер прослушивает порт " << port_ << "." << std::endl;

  // Обработка входящих подключений
  while (true) {
    int client = accept(socket_, nullptr, nullptr);
    if (client == -1) {
      throw std::runtime_error("Ошибка при подключении клиента!");
    }
    // Создание нового потока для обработки подключения
    client_threads_.push_back(std::thread(&Server::ClientThread, this, client));
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

// Получение текущих даты и времени
std::string Server::GetDateTime() {
  std::time_t now = std::time(nullptr);
  std::tm* dt = std::localtime(&now);
  std::stringstream ss;
  ss << std::put_time(dt, "%d:%m:%Y %H:%M");
  return ss.str();
}

// Метод для обработки подключений клиентов
void Server::ClientThread(int client) {
  std::cout << GetDateTime() << " Клиент номер " << client
            << " подключился к серверу." << std::endl;
  clients_.insert(client);
  char buffer[1024];
  int bytes_received;

  // Получение данных от клиента и отправка сообшения клиенту
  while ((bytes_received = recv(client, buffer, 1024, 0)) > 0) {
    std::string message(buffer, bytes_received);
    SendMessage(client, message);
  }

  if (bytes_received == 0) {
    std::cout << GetDateTime() << " Клиент номер " << client
              << " отключился от сервера." << std::endl;
  } else {
    throw std::runtime_error("Ошибка получения данных от клиента номер " +
                             std::to_string(client) + "!");
  }
  clients_.erase(client);
  close(client);
}

void Server::Stop() {
  close(socket_);
  for (std::thread& thread : client_threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}
