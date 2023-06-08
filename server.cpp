#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Create the server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Bind the server socket to a specific port
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Choose a suitable port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Failed to bind socket");
        close(serverSocket);
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 1) == -1) {
        perror("Failed to listen");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening for incoming connections..." << std::endl;

    // Accept incoming connections
    struct sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        perror("Failed to accept connection");
        close(serverSocket);
        return 1;
    }

    std::cout << "Client connected. Ready to receive keystrokes." << std::endl;

    // Receive and process data from the client
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            char key = buffer[i];

            // Process the received keystroke here
            std::cout << key;
            std::cout.flush();
        }
    }


    if (bytesRead == -1) {
        perror("Failed to receive data");
    }

    std::cout << "Connection closed. Server is shutting down." << std::endl;

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
