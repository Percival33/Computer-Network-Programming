// Created by Michał Sobiech on 20.11.2023

#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define ERROR_INVALID_ARGC 1
#define ERROR_FAILED_SOCKET_CREATION 2
#define ERROR_FAILED_SOCKET_BIND 3
#define ERROR_FAILED_TO_RECEIVE_A_MESSAGE 4
#define ERROR_FAILED_TO_SEND_A_MESSAGE 5

#define BUF_SIZE 512 // Buffer size for data
#define MAX_PAYLOAD_SIZE 508 // (512 - 4)
#define KEY_SIZE 2
#define VALUE_SIZE 2
#define MAX_PAIR_COUNT MAX_PAYLOAD_SIZE / (KEY_SIZE + VALUE_SIZE)\
#define PAIR_SIZE KEY_SIZE + VALUE_SIZE

#define RESPONSE_WAIT_TIME_S 1

typedef struct {
    uint16_t id;
    uint8_t status; // if different from 0 then err
} response_t;

// Args for send and receive functions
typedef struct {
    int sockfd;
    void *message_buffer;
    int message_buffer_length;
    struct sockaddr_in *client_address;
} message_s_or_r_args_t;

int send_message_to_client(message_s_or_r_args_t *args) {
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

int receive_message_from_client(message_s_or_r_args_t *args) {
    int data_length = recvfrom(
        args->sockfd,
        args->message_buffer, 
        args->message_buffer_length,
        0,
        (struct sockaddr*) args->client_address, 
        &(socklen_t){sizeof(args->client_address)}
    );
    if (data_length == -1) {
        printf("Failed to receive data\n");
        exit(ERROR_FAILED_TO_RECEIVE_A_MESSAGE);
    }
    return data_length;
}

typedef struct {
    message_s_or_r_args_t *send_message_args;
    bool message_received;
} timer_thread_args_t;

void *timer_thread_function(void *args) {
    timer_thread_args_t *args_parsed = (timer_thread_args_t*) args;
    while(true) {
        sleep(RESPONSE_WAIT_TIME_S);
        if (args_parsed->message_received) {
            // TODO delete this printf
            printf("timer thread ended\n");
            return NULL;
        }
        else {
            send_message_to_client(args_parsed->send_message_args);
        }
    }
}

typedef struct {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
} key_value_pair_t;

typedef struct {
    uint16_t pair_count;
    uint16_t packet_id;
    key_value_pair_t pairs[MAX_PAIR_COUNT];
} packet_data_t;

bool datagram_is_valid(packet_data_t *packet_data) {
    // Datagram is valid, if:
    // - After the declared pairs there are only zeroes,
    // - The number of pairs does not exceed the max number of
    // pairs that can fit in a packet
    
    // TODO parametrize

    int pair_count = packet_data->pair_count;

    // Size
    int max_pair_count = floor(MAX_PAYLOAD_SIZE / (KEY_SIZE + VALUE_SIZE));
    if (pair_count > max_pair_count) {
        printf("aaaa\n");
        return false;
    }

    // Check if the remaining are equal to 0.
    // IMPORTANT - THERE MUST ONLY BE FULL PAIRS
    char *key;
    char *value;
    for (int i = pair_count; i < max_pair_count; i++) {
        key = packet_data->pairs[i].key;
        if (!text_is_only_zeroes(key, sizeof(key))) {
            return false;
        }
        value = packet_data->pairs[i].value;
        if (!text_is_only_zeroes(value, sizeof(key))) {
            return false;
        }
    }
    return true;
}

bool text_is_only_zeroes(char *text, int text_length) {
    for (int i = 0; i < text_length; i++) {
        if (text[i] != '\0') {
            return false;
        }
    }
    return true;
}

bool response_is_valid(char buffer[], int expected_id) {
    // TODO parametrize
    int packet_id = ntohs(((uint16_t)buffer[0] << 8) + (uint16_t)buffer[1]);
    if (packet_id != expected_id) {
        return false;
    }
    int status_code = (int) buffer[2];
    if (status_code != 0) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARGC);
    }

    int port = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd  < 0) {
        perror("Failed to create a socket");
        exit(ERROR_FAILED_SOCKET_CREATION);
    }

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind a socket");
        exit(ERROR_FAILED_SOCKET_BIND);
    }

    packet_data_t packet_data;
    char client_ip_str[INET_ADDRSTRLEN];
    while(true) {
        message_s_or_r_args_t receive_msg_from_clients_args = {
            sockfd,
            &packet_data,
            sizeof(packet_data),
            &client_address
        };
        receive_message_from_client(&receive_msg_from_clients_args);

        // Parse the client's address
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf("Data received from %s:%d\n", client_ip_str, ntohs(client_address.sin_port));

        if (datagram_is_valid(&packet_data)) {
            printf("Number of pairs: %d\n", packet_data.pair_count);
            printf("Packet id: %d\n", packet_data.packet_id);
            printf("Pairs: \n");
            for (int i = 0; i < packet_data.pair_count; i++) {
                printf("%s:%s\n", packet_data.pairs[i].key, packet_data.pairs[i].value);
            }
        }

        response_t response_to_client = {
            (uint16_t) packet_data.packet_id,
            0
        };
        message_s_or_r_args_t send_res_to_client_args = {
            sockfd,
            &response_to_client,
            sizeof(response_to_client),
            &client_address
        };
        send_message_to_client(&send_res_to_client_args);

        // Wait for the response to the response

        // Start a timer on one thread. 
        // It will periodically resend the message...
        response_t response_from_client;
        pthread_t timer_thread;
        timer_thread_args_t timer_args = {
            &(message_s_or_r_args_t){
                sockfd,
                &response_from_client,
                sizeof(response_from_client),
                &client_address
            },
            false
        };
        pthread_create(&timer_thread, NULL, timer_thread_function, (void *)&timer_args);
    
        // ... and start listening for a response from the main thread.
        message_s_or_r_args_t receive_res_from_client_args = {
            sockfd,
            &response_from_client,
            sizeof(response_from_client),
            &client_address
        };
        int res_data_length = receive_message_from_client(&receive_res_from_client_args);
        timer_args.message_received = true;
        printf("Handshake completed\n");
    }
    exit(0);
}