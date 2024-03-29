//
// Created by Michał Sobiech on 30.11.2023
//


#ifndef ONE_USE_SOCKET_H
#define ONE_USE_SOCKET_H

#include "common.h"
#include "message_utils.h"

int create_one_use_socket();

// Create a one use socket to send a packet
void one_use_socket_send_message(
    message_contents_t *message_contents,
    struct sockaddr_in *client_address
);

// Create a one use socket to receive a packet
int one_use_socket_receive_message(
    message_contents_t *message_contents,
    struct sockaddr_in *client_address
);

// Resender that uses a one use socket
void *one_use_socket_resender(void *args);

#endif
