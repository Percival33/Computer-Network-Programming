#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "node.h"
#include "serialize.h"

#define PORT 8888
#define BACKLOG 1  // Number of pending connections queue will hold
#define RCVBUF_SIZE_KB 1024
#define RCVBUF_SIZE RCVBUF_SIZE_KB * KB 

void start_server(const char *host, int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    uint8_t buffer[RCVBUF_SIZE];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(host);
    address.sin_port = htons(port);

    // Binding the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started, listening on %s:%d\n", host, port);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Change the receive buffer size
        long int rcvbufSize = RCVBUF_SIZE;
        if (setsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, &rcvbufSize, sizeof(rcvbufSize)) != 0) {
            perror("setsockopt SO_RCVBUF failed");
            exit(EXIT_FAILURE);
        }

        printf("Connected by %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        while (1) {
            // int bytes_read = read(new_socket, buffer, sizeof(buffer));
            int bytes_read = read(new_socket, buffer, sizeof(buffer));
            if (bytes_read == 0) {
                break;
            }

            float bytes_read_kB_formatted = ((float) bytes_read)/((float)KB);
            printf("Read %.2f kB of data.\n", bytes_read_kB_formatted);
        
            // Artificial delay
            sleep(1.5f);
        }

        close(new_socket);
    }
}

int main() {
    printf("Z2.3a C server\n");

    start_server("0.0.0.0", PORT);
    return 0;
}
