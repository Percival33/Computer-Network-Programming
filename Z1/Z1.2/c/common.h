//
// Created by Marcin Jarczewski on 21/11/2023.
//

#ifndef COMPUTER_NETWORK_PROGRAMMING_TODO_H
#define COMPUTER_NETWORK_PROGRAMMING_TODO_H
#include <stdbool.h>
// Error codes
#define OK 0
#define ERROR_INVALID_ARGC 1
#define ERROR_FAILED_SOCKET_CREATION 2
#define ERROR_FAILED_SOCKET_BIND 3
#define ERROR_FAILED_TO_RECEIVE_A_MESSAGE 4
#define ERROR_FAILED_TO_SEND_A_MESSAGE 5

// Constants
#define BUFFER_SIZE 512 // Buffer size for data
#define HEADER_SIZE 4 // sizeof id + size
#define MAX_PAYLOAD_SIZE (BUFFER_SIZE - HEADER_SIZE)
#define KEY_SIZE 2
#define VALUE_SIZE 2
#define PAIR_SIZE (KEY_SIZE + VALUE_SIZE)
#define MAX_PAIR_COUNT (MAX_PAYLOAD_SIZE/PAIR_SIZE)
#define RESPONSE_WAIT_TIME_S 1

// Custom types
typedef struct {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
} key_value_pair_t;

typedef struct {
    uint16_t id;
    uint16_t count;
    key_value_pair_t pairs[MAX_PAYLOAD_SIZE];
} data_t;

typedef struct {
    uint16_t id;
    uint8_t status;
} response_t;

// Args for send and receive functions
typedef struct {
    int sockfd;
    void *message_buffer;
    int message_buffer_length;
    struct sockaddr_in *client_address;
} message_args_t;

typedef struct {
    message_args_t *send_message_args;
    bool message_received;
} resender_args_t;

#endif //COMPUTER_NETWORK_PROGRAMMING_TODO_H
