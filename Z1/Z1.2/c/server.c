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
#include "common.h"

int send_message_to_client(message_args_t *args) {
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

int receive_message_from_client(message_args_t *args) {
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

void *resender(void *args) {
    resender_args_t *args_parsed = (resender_args_t*) args;
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

bool text_is_only_zeroes(char *text, int text_length) {
    for (int i = 0; i < text_length; i++) {
        if (text[i] != '\0') {
            return false;
        }
    }
    return true;
}

bool datagram_is_valid(data_t *packet_data) {
    // Datagram is valid, if:
    // - After the declared pairs there are only zeroes,
    // - The number of pairs does not exceed the max number of
    // pairs that can fit in a packet
    
    // TODO parametrize

    int count = packet_data->count;

    // Size
    if (count > MAX_PAIR_COUNT) {
        printf("aaaa\n");
        return false;
    }

    // Check if the remaining are equal to 0.
    // IMPORTANT - THERE MUST ONLY BE FULL PAIRS
    char *key;
    char *value;
    for (int i = count; i < MAX_PAIR_COUNT; i++) {
        key = packet_data->pairs[i].key;
        if (!text_is_only_zeroes(key, KEY_SIZE)) {
            return false;
        }
        value = packet_data->pairs[i].value;
        if (!text_is_only_zeroes(value, VALUE_SIZE)) {
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

void ntoh_on_packet_data(packet_data_t *packet_data) {
    packet_data->pair_count = ntohs(packet_data->pair_count);
    packet_data->packet_id = ntohs(packet_data->packet_id);
}

void ntoh_on_response(response_t *response) {
    response->id = ntohs(response->id);
    response->status = ntohs(response->status);
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

    data_t packet_data;
    char client_ip_str[INET_ADDRSTRLEN];
    while(true) {
        message_args_t receive_msg_from_clients_args = {
            sockfd,
            &packet_data,
            sizeof(packet_data),
            &client_address
        };
        receive_message_from_client(&receive_msg_from_clients_args);
        ntoh_on_packet_data(&packet_data);

        // Parse the client's address
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf("Data received from %s:%d\n", client_ip_str, ntohs(client_address.sin_port));

        // if (datagram_is_valid(&packet_data)) {
        //     printf("Number of pairs: %d\n", packet_data.pair_count);
        //     printf("Packet id: %d\n", packet_data.packet_id);
        //     printf("Pairs: \n");
        //     for (int i = 0; i < packet_data.pair_count; i++) {
        //         printf("%s:%s\n", packet_data.pairs[i].key, packet_data.pairs[i].value);
        //     }
        // }
        printf("Number of pairs: %d\n", packet_data.pair_count);
        printf("Packet id: %d\n", packet_data.packet_id);
        printf("Pairs: \n");
        char printable_key[KEY_SIZE + 1];
        char printable_value[VALUE_SIZE + 1]; 
        printable_key[KEY_SIZE] = '\0';
        printable_value[VALUE_SIZE] = '\0';
        for (int i = 0; i < packet_data.pair_count; i++) {
            memcpy(printable_key, packet_data.pairs[i].key, KEY_SIZE);
            memcpy(printable_value, packet_data.pairs[i].value, VALUE_SIZE);

            printf("%s:%s\n", printable_key, printable_value);
        }

        response_t response_to_client = {
            (uint16_t) packet_data.id,
            0
        };
        message_args_t send_res_to_client_args = {
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
        pthread_t resender_thread;
        resender_args_t timer_args = {
            &(message_args_t){
                sockfd,
                &response_from_client,
                sizeof(response_from_client),
                &client_address
            },
            false
        };
        pthread_create(&resender_thread, NULL, resender, (void *)&timer_args);
    
        // ... and start listening for a response from the main thread.
        message_args_t receive_res_from_client_args = {
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