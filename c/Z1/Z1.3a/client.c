//
// Created by Marcin Jarczewski on 18/11/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#define SERVER_IP "z1_server" // Server IP address
#define SERVER_PORT 8888 // Server port

typedef struct {
    char* payload;
} message_t;

int packet_size;

void fillMessage(message_t *msg, int packet_size) {
    if(msg->payload != NULL)
        free(msg->payload);

    msg->payload = (char*)malloc(packet_size * sizeof(char));
    memset(msg->payload, '\0', packet_size);
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
    alarm(5);

    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct hostent *he;
    struct in_addr **addr_list;

    if ((he = gethostbyname(SERVER_IP)) == NULL) {
        perror("gethostbyname");
        return 2;
    }

    addr_list = (struct in_addr **)he->h_addr_list;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr = *addr_list[0];

    message_t a;
    a.payload = NULL;
    for(packet_size = 2000; packet_size < (1<<16); packet_size++) {
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
