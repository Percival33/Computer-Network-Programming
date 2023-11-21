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
#define MAX_PAIR_COUNT MAX_PAYLOAD_SIZE / (KEY_SIZE + VALUE_SIZE)

#define RESPONSE_WAIT_TIME_S 1

typedef struct {
    int sockfd;
    char *message;
    int message_length;
    struct sockaddr_in *client_address;
} s_m_t_c_args_t;

int send_message_to_client(s_m_t_c_args_t *args) {
    int data_length = sendto(
        args->sockfd,
        args->message,
        args->message_length,
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

typedef struct {
    int sockfd;
    char *buffer;
    int buffer_length;
    struct sockaddr_in *client_address;
} r_m_f_c_args_t;

int receive_message_from_client(r_m_f_c_args_t *args) {
    int data_length = recvfrom(
        args->sockfd,
        args->buffer, 
        args->buffer_length,
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
    int sockfd;
    char *message;
    int message_length;
    struct sockaddr_in *client_address;
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
            s_m_t_c_args_t args = {
                args_parsed->sockfd,
                args_parsed->message,
                args_parsed->message_length,
                args_parsed->client_address
            };
            send_message_to_client(&args);
        }
    }
}

typedef struct {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
} key_value_pair_t;

typedef struct {
    int pair_count;
    int packet_id;
    key_value_pair_t pairs[MAX_PAIR_COUNT];
} packet_data_t;

bool datagram_is_valid(char buffer[], int buffer_length, packet_data_t *packet_data) {
    // Datagram is valid, if:
    // - After the declared pairs there are only zeroes,
    // - The number of pairs does not exceed the max number of
    // pairs that can fit in a packet
    
    // TODO parametrize

    for (int i = 0; i < buffer_length; i++) {
        printf("%d \n", buffer[i]);
    }

    // The first 2 bytes are the size
    uint16_t pair_count = ntohs(((uint16_t)buffer[0] << 8) + (uint16_t)buffer[1]);
    int max_pair_count = floor(MAX_PAYLOAD_SIZE / (KEY_SIZE + VALUE_SIZE));
    if (pair_count > max_pair_count) {
        // return false;
        ;
    }
    packet_data->pair_count = pair_count;

    // The next 2 bytes are the packet id
    uint16_t packet_id = ntohs(((uint16_t)buffer[2] << 8) + (uint16_t)buffer[3]);
    packet_data->packet_id = packet_id;

    char *key;
    char *value;
    // TODO parmetrize current_byte_no
    int current_byte_no = 4;
    for (int i = 0; i < 1; i++) {
        key = packet_data->pairs[i].key;
        value = packet_data->pairs[i].value;
        strncpy(key, &buffer[current_byte_no], sizeof(key));
        strncpy(value, &buffer[current_byte_no + sizeof(key)], sizeof(key));
        current_byte_no += sizeof(key) + sizeof(value);
    }

    // Check if the remaining bytes are equal to 0.
    for (int i = current_byte_no; i < BUF_SIZE; i++) {
        if (buffer[i] != 0) {
            // return false;
            ;
        }
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

    char buffer[BUF_SIZE];
    char client_ip_str[INET_ADDRSTRLEN];
    while(true) {
        r_m_f_c_args_t receive_req_args = {
            sockfd,
            buffer,
            sizeof(buffer),
            &client_address
        };
        int req_data_length = receive_message_from_client(&receive_req_args);

        // Parse the client's address
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf("Data received from %s:%d\n", client_ip_str, ntohs(client_address.sin_port));

        packet_data_t packet_data;
        // if (datagram_is_valid(buffer, sizeof(buffer), &packet_data)) {
        datagram_is_valid(buffer, sizeof(buffer), &packet_data);
        if (true) {
            printf("Number of pairs: %d\n", packet_data.pair_count);
            printf("Packet id: %d\n", packet_data.packet_id);
            printf("Pairs: \n");
            for (int i = 0; i < packet_data.pair_count; i++) {
                printf("%s:%s", packet_data.pairs[i].key, packet_data.pairs[i].value);
            }
        }

        char response[] = "test";
        s_m_t_c_args_t first_response_args = {
            sockfd,
            response,
            sizeof(response),
            &client_address
        };
        send_message_to_client(&first_response_args);

        // Wait for the response to the response

        // Start a timer on one thread. 
        // It will periodically resend the message...
        pthread_t timer_thread;
        timer_thread_args_t timer_args = {
            sockfd,
            response,
            sizeof(response),
            &client_address,
            false
        };
        pthread_create(&timer_thread, NULL, timer_thread_function, (void *)&timer_args);
    
        // ... and start listening for a response toon the main thread.
        r_m_f_c_args_t receive_res_args = {
            sockfd,
            buffer,
            sizeof(buffer),
            &client_address
        };
        int res_data_length = receive_message_from_client(&receive_res_args);
        timer_args.message_received = true;
        printf("Handshake completed\n");
    }
    exit(0);
}