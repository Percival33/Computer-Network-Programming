//
// Created by Marcin Jarczewski on 18/11/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include "common.h"

#define SERVER_IP "127.0.0.1" // Server IP address
#define SERVER_PORT 8888 // Server port

void fillMessage(data_t *msg, uint16_t id, uint16_t count, key_value_pair_t payload[]) {
    msg->id = htons(id);
    msg->count = htons(count);

    int payloadLength = PAIR_SIZE * count;
    assert(payloadLength <= MAX_PAYLOAD_SIZE);

    memset(msg->pairs, '\0', MAX_PAYLOAD_SIZE);
    memcpy(msg->pairs, payload, payloadLength);
}

void fillPairs(key_value_pair_t pairs[], int size) {
    if (size > MAX_PAIR_COUNT) {
        printf(LOG_ERROR "Datagram is not valid. MAX_PAIR_COUNT(%d) exceeded got: %d\n", MAX_PAIR_COUNT, size);
        exit(ERROR_INVALID_ARG);
    }

    // filling all bytes to make constant size of datagram
    for(int i = 0; i < MAX_PAIR_COUNT; i++) {
        memset(pairs[i].key, '\0', KEY_SIZE);
        memset(pairs[i].value, '\0', VALUE_SIZE);
    }

    for(int i = 0; i < size; i++) {
        char key[] = {'A', 'A'};
        char value[] = {i/10 + '0', i%10 + '0'};

        strncpy(pairs[i].key, key, sizeof(key));
        strncpy(pairs[i].value, value, sizeof(value));
    }
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    char buffer[BUFFER_SIZE];
    data_t data, maxData;
    key_value_pair_t pairs[MAX_PAIR_COUNT], maxPairs[MAX_PAIR_COUNT];
    message_args_t message;

    fillPairs(pairs, 2);
    fillPairs(maxPairs, MAX_PAIR_COUNT);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    fillMessage(&data, 1, 2, pairs);
    fillMessage(&maxData, 2, MAX_PAIR_COUNT, maxPairs);

    message.message_buffer_length = sizeof(data);
    message.message_buffer = (void*) &data;
    message.sockfd = sockfd;
    message.to_address = &serverAddr;
    send_message(&message);

    printf(LOG_INFO"Sending second message\n");
    message.message_buffer_length = sizeof(maxData);
    message.message_buffer = (void*) &maxData;
    send_message(&message);
    printf(LOG_INFO"Second message was sent\n");

    // Close the socket
    close(sockfd);
    return 0;
}
