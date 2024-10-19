// Copyright 2024 Asfand Ali
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

int main() {
  // Create socket
  int listening = socket(AF_INET, SOCK_STREAM, 0);
  if (listening == -1) {
    cerr << "Failed to create socket";
    return -1;
  }

  // Bind socket to IP/Port
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(54000);
  inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
  if (bind(listening, reinterpret_cast<sockaddr *>(&hint), sizeof(hint)) ==
      -1) {
    cerr << "Failed to bind to IP/Port";
    return -2;
  }

  // Start socket listening
  if (listen(listening, SOMAXCONN) == -1) {
    cerr << "Failed to listen";
    return -3;
  }

  // Accept incoming connection
  sockaddr_in client;
  socklen_t clientSize = sizeof(client);
  char host[NI_MAXHOST];
  char svc[NI_MAXSERV];
  int clientSocket =
      accept(listening, reinterpret_cast<sockaddr *>(&client), &clientSize);
  if (clientSocket == -1) {
    cerr << "Client failed to connect";
    return -4;
  }

  // Close listening socket
  close(listening);
  memset(host, 0, NI_MAXHOST);
  memset(svc, 0, NI_MAXSERV);

  int result =
      getnameinfo(reinterpret_cast<sockaddr *>(&client), sizeof(client), host,
                  NI_MAXHOST, svc, NI_MAXSERV, 0);
  if (result) {
    cout << host << " connected on " << svc << endl;
  } else {
    inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
    cout << host << " connected on " << ntohs(client.sin_port) << endl;
  }

  char buf[4096];
  while (true) {
    // Clear buffer
    memset(buf, 0, 4096);
    int bytesRecv = recv(clientSocket, buf, 4096, 0);
    if (bytesRecv == -1) {
      cerr << "There was a connection issue" << endl;
      break;
    }
    if (bytesRecv == 0) {
      cout << "The client disconnected" << endl;
    }
    // Display message
    cout << "Received: " << string(buf, 0, bytesRecv) << endl;
    // Send message back to client
    send(clientSocket, buf, bytesRecv + 1, 0);
  }
  close(clientSocket);
  return 0;
}
