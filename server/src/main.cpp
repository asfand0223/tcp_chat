// Copyright 2024 Asfand Ali
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

const int PORT = 54000;
const int BUFFER_SIZE = 4096;

void handle_client(int clientSocket) {
  char buffer[BUFFER_SIZE];

  while (true) {
    // Clear buffer
    memset(buffer, 0, BUFFER_SIZE);
    int bytesRecv = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRecv == -1) {
      std::cerr << "There was a connection issue" << std::endl;
      break;
    }
    if (bytesRecv == 0) {
      std::cout << "The client disconnected" << std::endl;
      break;
    }
    // Display message
    std::cout << "Received: " << std::string(buffer, 0, bytesRecv) << std::endl;
    // Send message back to client
    send(clientSocket, buffer, bytesRecv + 1, 0);
  }
  close(clientSocket);
}

int main() {
  // Create socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Failed to create socket";
    return -1;
  }

  // Bind socket to IP/Port
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(PORT);
  inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
  if (bind(server_socket, reinterpret_cast<sockaddr *>(&hint), sizeof(hint)) ==
      -1) {
    std::cerr << "Failed to bind to IP/Port";
    return -2;
  }

  // Start listening
  if (listen(server_socket, SOMAXCONN) == -1) {
    std::cerr << "Failed to listen";
    return -3;
  }

  std::vector<std::thread> clientThreads;

  while (true) {
    // Accept incoming connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int clientSocket = accept(
        server_socket, reinterpret_cast<sockaddr *>(&client), &clientSize);
    if (clientSocket == -1) {
      std::cerr << "Client failed to connect";
      return -4;
    }
    std::cout << "A new client connected" << std::endl;

    // Add new clientThread and then join it if possible
    clientThreads.emplace_back(handle_client, clientSocket);
    clientThreads.back().detach();
  }

  close(server_socket);

  return 0;
}
