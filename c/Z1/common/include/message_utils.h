//
// Created by Marcin Jarczewski on 21/11/2023.
//

#ifndef MESSAGE_UTILS_H
#define MESSAGE_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

// Args for send and receive functions
typedef struct {
    int sockfd;
    void *message_buffer;
    int message_buffer_length;
    struct sockaddr_in *to_address;
} message_args_t;

typedef struct {
    message_args_t *send_message_args;
    bool message_received;
} resender_args_t;

typedef struct {
    void *data;
    int length;
} message_contents_t;

int send_message(message_args_t *args);
int receive_message(message_args_t *args);
void *resender(void *args);

#endif