// Copyright 2024 Asfand Ali
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
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

  while (true) {
    // Create data to send
    std::string message;
    std::cout << "Enter message to send (type 'exit' to quit): ";
    std::getline(std::cin, message);

    // Check for exit condition
    if (message == "exit") {
      std::cout << "Exiting client." << std::endl;
      break;
    }

    // Send data
    send(clientSocket, message.c_str(), message.length(), 0);

    // Receive data from server
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0) {
      std::cerr << "Error receiving response" << std::endl;
      break;
    } else if (bytesReceived == 0) {
      std::cout << "Server disconnected" << std::endl;
      break;
    } else {
      std::cout << "Server response: " << std::string(buffer, bytesReceived)
                << std::endl;
    }
  }

  // Close the socket
  close(clientSocket);
  return 0;
}
