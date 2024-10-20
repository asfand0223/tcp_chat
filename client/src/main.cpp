// Copyright 2024 Asfand Ali
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  // Create socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return -1;
  }
  // Connect socket to IP/Port
  sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(54000);
  inet_pton(AF_INET, "0.0.0.0", &server_address.sin_addr);
  if (connect(clientSocket, reinterpret_cast<sockaddr *>(&server_address),
              sizeof(server_address)) == -1) {
    std::cerr << "Failed to connect to server" << std::endl;
    close(clientSocket);
    return -2;
  }
  // Create data to send
  const char *message = "Hello, server!";
  // Send data
  send(clientSocket, message, strlen(message), 0);
  // Receive data from server
  char buffer[4096];
  memset(buffer, 0, sizeof(buffer));
  int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesReceived < 0) {
    std::cerr << "Error receiving response" << std::endl;
    return -3;
  } else {
    std::cout << "Server response: " << buffer << std::endl;
  }
  close(clientSocket);
  return 1;
}
