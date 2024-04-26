#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <mutex>
#include <thread>
#include <unistd.h>

std::vector<std::string> cycle_buffer(1024 * 1024);

std::mutex bufferMutex;

volatile sig_atomic_t must_exit = 0;
volatile sig_atomic_t num = 0;

void handle_sigterm(int signum) {
  must_exit = 1;
  if (num == 0) {
    exit(0);
  }
}

void handle(int client_socket) {
  char buffer[1024];
  while (true) {
    int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
      break;
    }
    std::lock_guard<std::mutex> lock(bufferMutex);
    cycle_buffer.push_back(std::string(buffer, bytes));
    std::string response = "Got message";
    send(client_socket, response.c_str(), response.size(), 0);
  }

  close(client_socket);
}

int main() {
  struct sigaction action_term;
  memset(&action_term, 0, sizeof(action_term));
  action_term.sa_handler = handle_sigterm;
  action_term.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &action_term, NULL);
  sigaction(SIGINT, &action_term, NULL);

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    std::cout << "Error when creating socket";
    return -1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);

  if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    std::cout << "Error when binding";
    return -1;
  }

  if (listen(socket_fd, SOMAXCONN) == -1) {
    std::cout << "Error when listening";
    return -1;
  }

  while (true) {
    if (must_exit) {
      break;
    }
    int client_socket = accept(socket_fd, nullptr, nullptr);
    if (client_socket == -1) {
      std::cout << "Error when accepting";
      continue;
    }

    std::thread t(handle, client_socket);
    t.detach();
  }

  close(socket_fd);
  return 0;
}