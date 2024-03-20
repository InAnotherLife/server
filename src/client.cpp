#include "client.h"

Client::Client(const std::string &address, int port)
    : address_(address), port_(port) {
  // Создание сокета
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    throw std::runtime_error("Ошибка при создании сокета!");
  }

  // Настройка адреса сервера
  server_.sin_family = AF_INET;
  server_.sin_port = htons(port_);
  inet_pton(AF_INET, address_.c_str(), &server_.sin_addr);
}

void Client::Connect() {
  // Установка соединения с сервером
  if (connect(socket_, (struct sockaddr *)&server_, sizeof(server_)) < 0) {
    throw std::runtime_error("Ошибка подключения к серверу!");
  }

  std::cout << "Подключение к серверу." << std::endl;

  while (true) {
    std::cout
        << "Введите сообщение для сервера или клиента через =id клиента: ";
    std::string message;
    std::getline(std::cin, message);
    if (std::cin.fail()) {
      throw std::runtime_error("Ошибка ввода!");
    }

    // Отправка сообщения на сервер
    send(socket_, message.c_str(), message.length(), 0);

    // Получение ответа от сервера
    char buffer[1024];
    int bytes_received = recv(socket_, buffer, 1024, 0);
    if (bytes_received > 0) {
      std::cout << "Ответ сервера:" << std::endl;
      std::cout << std::string(buffer, bytes_received) << std::endl;
    } else {
      throw std::runtime_error("Ошибка при получении ответа от сервера!");
    }
  }
}

void Client::Disconnect() { close(socket_); }
