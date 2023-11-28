//
// Created by Marcin Jarczewski on 18/11/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>


#define SERVER_IP "127.0.0.1" // Server IP address
#define SERVER_PORT 8888 // Server port

#define BUF_SIZE 2048 // Buffer size for data
#define MAX_PAYLOAD_SIZE 508 // (512 - 4)
#define KEY_SIZE 2
#define VALUE_SIZE 2

typedef struct {
    char payload[BUF_SIZE];
} message_t;

void fillMessage(message_t *msg, int packet_size) {
    memset(msg->payload, '\0', BUF_SIZE);
    assert(packet_size < BUF_SIZE);
    for(int i = 0; i < packet_size; i++) {
        msg->payload[i] = 'A';
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
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
    int N[] = {1, 100, 200, 1000, 1050, 2000};
    for(int x = 0; x < sizeof(N)/sizeof(N[0]); x++) {
        fillMessage(&a, N[x]);

        sendto(sockfd, &a, N[x], 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        printf("Sent packet of %d bytes to server\n", N[x]);

        char response[5];
        recvfrom(sockfd, &response, 5, 0, NULL, 0);
        int len = ntohs(atoi(response));
        printf("resp: %d\n", len);
    }


//    printf("Received from server: %s\n", response);

    // Close the socket
    close(sockfd);
    return 0;
}
