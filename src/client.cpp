#include "client.h"

Client::Client(std::string server_ip, int server_port)
    : server_ip_(server_ip), server_port_(server_port) {
  // Создание сокета
  client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket_ < 0) {
    std::cerr << "Ошибка при создании сокета!" << std::endl;
    exit(1);
  }

  // Настройка адреса сервера
  server_.sin_family = AF_INET;
  server_.sin_port = htons(server_port_);
  inet_pton(AF_INET, server_ip_.c_str(), &server_.sin_addr);
}

void Client::Connect() {
  // Установка соединения с сервером
  if (connect(client_socket_, (struct sockaddr *)&server_, sizeof(server_)) <
      0) {
    std::cerr << "Ошибка подключения к серверу!" << std::endl;
    exit(1);
  }

  std::cout << "Подключение к серверу." << std::endl;

  std::string message;
  std::cout << "Введите сообщение: ";
  std::cin >> message;
  if (std::cin.fail()) {
    std::cerr << "Ошибка ввода!" << std::endl;
    exit(1);
  }

  send(client_socket_, message.c_str(), message.length(), 0);

  // Получение ответа от сервера
  char buffer[1024];
  int bytes_received = recv(client_socket_, buffer, 1024, 0);
  if (bytes_received > 0) {
    std::cout << "Ответ сервера:" << std::endl;
    std::cout << std::string(buffer, bytes_received) << std::endl;
  } else if (bytes_received == 0) {
    std::cout << "Отключение от сервера." << std::endl;
  } else {
    std::cerr << "Ошибка при получении ответа от сервера!" << std::endl;
  }
}

void Client::Disconnect() { close(client_socket_); }
