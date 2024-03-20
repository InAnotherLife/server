CC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wextra
SOURCES=notes.cpp menu.cpp main.cpp

all: server client

server:
	$(CC) $(CFLAGS) -c server.cpp main_server.cpp
	$(CC) server.o main_server.o -o server

client:
	$(CC) $(CFLAGS) -c client.cpp main_client.cpp
	$(CC) client.o  main_client.o -o client

clang-format:
	clang-format -i server.cpp server.h main_server.cpp client.cpp client.h main_client.cpp

linter:
	clang-format -n server.cpp server.h main_server.cpp client.cpp client.h main_client.cpp

cppcheck:
	cppcheck --std=c++17 --language=c++ --enable=all --suppress=missingIncludeSystem *.cpp *.h

clean:
	rm -f ./client ./server ./*.o ./*.a
	rm -rf ./.vscode
