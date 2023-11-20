//
// Created by Marcin Jarczewski on 18/11/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SERVER_IP "127.0.0.1" // Server IP address
#define SERVER_PORT 8888 // Server port
#define BUF_SIZE 1024 // Buffer size for data
#define STR_SIZE 2
typedef struct {
    uint16_t size;
    char key[STR_SIZE];
    char value[STR_SIZE];
} message_t;

int main(int argc, char* argv[]) {
    if(argc != 1) {
        perror("Arguments error");
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUF_SIZE];
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    message_t a;
    a.size = htons(1);
    char str[STR_SIZE];
    memset(a.key, '\0', STR_SIZE);
    strncpy(a.key, "A", STR_SIZE - 1);
    memset(a.value, '\0', STR_SIZE);
    strncpy(a.value, "1", STR_SIZE - 1);

    memcpy(buffer, &a, sizeof(a));
    sendto(sockfd, buffer, sizeof(a), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    printf("Data sent to server\n");

    addr_size = sizeof(serverAddr);
    printf("%d\n", sizeof(buffer));

    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverAddr, &addr_size);
    for (int i = 0; i < sizeof(buffer); i++) {
        printf("%d ", (int)buffer[i]);
    }
    printf("\n");
    printf("Received from server: %s\n", buffer);

    // Close the socket
    close(sockfd);
    return 0;
}
