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

#define ERROR_INVALID_ARGC 1
#define ERROR_FAILED_SOCKET_CREATION 2
#define ERROR_FAILED_SOCKET_BIND 3
#define ERROR_FAILED_TO_RECEIVE_A_MESSAGE 4
#define ERROR_FAILED_TO_SEND_A_MESSAGE 5

#define PAIR_SIZE 4
#define BUFFER_SIZE 1024

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

// TODO struct for send_message_to_client args

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

    char buffer[BUFFER_SIZE];
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

        // The first 2 bytes are the size
        uint16_t pair_count = ntohs(((uint16_t)buffer[0] << 8) + (uint16_t)buffer[1]);

        printf("Number of pairs: %d\n", pair_count);

        int current_byte_no = 2;
        char key[PAIR_SIZE/2];
        char value[PAIR_SIZE/2];
        for (int i = 0; i < 1; i++) {
            strncpy(key, &buffer[current_byte_no], sizeof(key));
            strncpy(value, &buffer[current_byte_no + sizeof(key)], sizeof(key));
            current_byte_no += sizeof(key) + sizeof(value);
            printf("%s : %s\n", key, value);
        }

        // TODO check if datagram is valid

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