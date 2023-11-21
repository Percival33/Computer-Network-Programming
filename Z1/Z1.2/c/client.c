//
// Created by Marcin Jarczewski on 18/11/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include "common.h"

#define SERVER_IP "127.0.0.1" // Server IP address
#define SERVER_PORT 8888 // Server port

void fillMessage(data_t *msg, uint16_t id, uint16_t count, key_value_pair_t payload[]) {
    msg->id = htons(id);
    msg->count = htons(count);

    int payloadLength = PAIR_SIZE * count;
//    assert(payloadLength < MAX_PAYLOAD_SIZE);

    memset(msg->pairs, '\0', MAX_PAYLOAD_SIZE);
    strncpy(msg->pairs, payload, payloadLength);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        perror("Arguments error");
        exit(1);
    }

    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
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

    data_t a;
    fillMessage(&a, 1, 2, "A 1 B 2"); // A: 1, B: 2

    sendto(sockfd, &a, sizeof(a), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    printf("Data sent to server\n");

    addr_size = sizeof(serverAddr);
    response_t response;
    recvfrom(sockfd, &response, 3, 0, (struct sockaddr *) &serverAddr, &addr_size);

    printf("resp: id(%d) status(%d)\n", htons(response.id), htons(response.status));
    if(response.status == 0)
        sendto(sockfd, &response, sizeof(response), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // wysylam
    // czekam na odpowiedz, jak jej nie dostane po czasie X to wyslij jeszcze raz i zresetuj timer
    // jak dostaniesz odpowiedz to zakoncz program

//    printf("Received from server: %s\n", response);

    // Close the socket
    close(sockfd);
    return 0;
}
