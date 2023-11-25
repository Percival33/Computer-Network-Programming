#ifndef ONE_USE_SOCKET_H
#define ONE_USE_SOCKET_H

#include "common.h"

typedef struct {
    data_t *data;
    struct sockaddr_in *client_address;
} message_and_client_address_t;

// Create a one use socket to send a packet
void one_use_socket_send_message(int port, datagram_and_client_address_t *datagram_and_client_address);

// Create a one use socket to receive a packet
void one_use_socket_receive_message(int port, response_and_client_address_t *response_and_client_address);

// Resender that uses a one use socket
void *one_use_socket_resender(void *args);

#endif