//
// Created by Marcin Jarczewski on 18/11/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define SERVER_IP "127.0.0.1" // Server IP address
#define SERVER_PORT 8888 // Server port

#define BUF_SIZE 2048 // Buffer size for data
#define MAX_PAYLOAD_SIZE 508 // (512 - 4)
#define KEY_SIZE 2
#define VALUE_SIZE 2

typedef struct {
    char* payload;
} message_t;

int packet_size;

void fillMessage(message_t *msg, int packet_size) {
    if(msg->payload != NULL)
        free(msg->payload);

    msg->payload = (char*)malloc(packet_size * sizeof(char));
    memset(msg->payload, '\0', BUF_SIZE);
    for(int i = 0; i < packet_size; i++) {
        msg->payload[i] = 'A';
    }
}

void handle_idle(int sig) {
    if (sig == SIGALRM) {
        printf("Timeout expired. Maximum packet size is %d bytes.\n", packet_size);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        perror("Arguments error");
        exit(1);
    }
    signal(SIGALRM, handle_idle);
    alarm(1);

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
    a.payload = NULL;
    for(packet_size = 200; packet_size < (1<<16); packet_size++) {
        fillMessage(&a, packet_size);

        sendto(sockfd, &a, packet_size, 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        printf("Sent packet of %d bytes to server. Waiting for a response...\n", packet_size);

        char response[5];
        int data_received = recvfrom(sockfd, &response, 5, 0, NULL, 0);
        if (data_received <= 0) {
            perror("Recvfrom failed:");
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}
