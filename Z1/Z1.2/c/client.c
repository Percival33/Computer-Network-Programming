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
int NO_PAIRS = 2;

void fillMessage(data_t *msg, uint16_t id, uint16_t count, key_value_pair_t payload[]) {
    msg->id = htons(id);
    msg->count = htons(count);

    int payloadLength = PAIR_SIZE * count;
//    assert(payloadLength < MAX_PAYLOAD_SIZE);

    memset(msg->pairs, '\0', MAX_PAYLOAD_SIZE);
    memcpy(msg->pairs, payload, payloadLength);
}

void fillPairs(key_value_pair_t pairs[], int size) {
    for(int i = 0; i < size; i++) {
        memset(pairs[i].key, '\0', KEY_SIZE);
        memset(pairs[i].value, '\0', VALUE_SIZE);
    }

    for(int i = 0; i < size; i++) {
        char* key = "AA";
        char value[] = {i/10 + '0', i%10 + '0'};


//        sprintf(value, "%c%c", i/10 + '0', i%10 + '0');

        strncpy(pairs[i].key, key, 2);
        strncpy(pairs[i].value, value, 2);
    }
}

int send_message(message_args_t *args) {
    int data_length = sendto(
        args->sockfd,
        args->message_buffer,
        args->message_buffer_length,
        0,
        (struct sockaddr*) args->client_address,
        sizeof(*args->client_address)
    );
    if (data_length == -1) {
        perror("Failed to send a message to the client");
        exit(ERROR_FAILED_TO_SEND_A_MESSAGE);
    }
    return data_length;
}

void *resender(void *args) {
    printf("Resender thread has started.\n");
    resender_args_t *args_parsed = (resender_args_t*) args;
    while (true) {
        sleep(RESPONSE_WAIT_TIME_S);
        if (args_parsed->message_received) {
            printf("Resender thread has ended.\n");
            return NULL;
        }
        send_message(args_parsed->send_message_args);
    }
}

void send_message_with_handshake(message_args_t *message) {
    resender_args_t timer_args;
    response_t response;
    pthread_t resender_thread;

    // send message and if response is absent then periodically resend the message
    timer_args.send_message_args = message;
    timer_args.message_received = false;
    send_message(message);
    pthread_create(&resender_thread, NULL, resender, (void *)&timer_args);
    printf("Data sent to server\n");

    recvfrom(message->sockfd, &response, 3, 0, (struct sockaddr *) &message->client_address, sizeof(message->client_address));
    printf("resp: id(%d) status(%d)\n", htons(response.id), htons(response.status));
    timer_args.message_received = true;

    if(response.status == OK) {
        send_message(&(message_args_t){
            message->sockfd,
            (void *)&response,
            sizeof(response),
            message->client_address,
        });
//        sendto(message->sockfd, &response, sizeof(response), 0, (struct sockaddr *) &message->client_address, sizeof(serverAddr));
        printf("Handshake completed.\n");
    }
    else {
        perror("Response status error");
        exit(response.status);
    }
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    char buffer[BUFFER_SIZE];
    data_t data, maxData;
    key_value_pair_t pairs[NO_PAIRS], maxPairs[MAX_PAIR_COUNT];
    message_args_t message;

    fillPairs(pairs, NO_PAIRS);
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

    // wysylam
    message.message_buffer_length = sizeof(data);
    message.message_buffer = (void*) &data;
    message.sockfd = sockfd;
    message.client_address = &serverAddr;

    send_message_with_handshake(&message);


    // jak dostaniesz odpowiedz to zakoncz program

//    printf("Received from server: %s\n", response);

    // Close the socket
//    pthread_join(resender_thread, NULL);
    close(sockfd);
    return 0;
}
