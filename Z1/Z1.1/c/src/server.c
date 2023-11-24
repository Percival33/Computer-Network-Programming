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
        printf(LOG_ERROR "Datagram is not valid. MAX_PAIR_COUNT(%d) exceeded got: %d\n", MAX_PAIR_COUNT, count);
        return false;
    }

    // Check if the remaining are equal to 0.
    // IMPORTANT - THERE MUST ONLY BE FULL PAIRS
    char *key;
    char *value;
    for (int i = count; i < MAX_PAIR_COUNT; i++) {
        key = packet_data->pairs[i].key;
        if (!text_is_only_zeroes(key, KEY_SIZE)) {
            printf(LOG_ERROR"Datagram is not valid. Error parsing key[%d]\n", i);
            return false;
        }
        value = packet_data->pairs[i].value;
        if (!text_is_only_zeroes(value, VALUE_SIZE)) {
            printf(LOG_ERROR"Datagram is not valid. Error parsing value[%d]=(%s)\n", i, value);
            return false;
        }
    }
    return true;
}

bool response_is_valid(char buffer[], int expected_id) {
    // TODO parametrize
    int packet_id = ntohs(((uint16_t)buffer[0] << 8) + (uint16_t)buffer[1]);
    if (packet_id != expected_id) {
        printf(LOG_ERROR"parsing datagram id. Got (%d) expected(%d)\n", packet_id, expected_id);
        return false;
    }
    int status_code = (int) buffer[2];
    if (status_code != 0) {
        printf(LOG_ERROR"status. Got (%d) expected(%d)\n", status_code, 0);
        return false;
    }
    return true;
}

void ntoh_on_packet_data(data_t *packet_data) {
    packet_data->count = ntohs(packet_data->count);
    packet_data->id = ntohs(packet_data->id);
}

void ntoh_on_response(response_t *response) {
    response->id = ntohs(response->id);
    response->status = ntohs(response->status);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    int port = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd  < 0) {
        perror("Failed to create a socket");
        exit(ERROR_FAILED_SOCKET_CREATION);
    }
    int recvBufferSize = 1024 * 1024 * 2; // example buffer size: 2 MB

    // Set the option
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvBufferSize, sizeof(recvBufferSize)) < 0) {
        perror("setsockopt SO_RCVBUF failed");
        // Handle error
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
        receive_message(&receive_msg_from_clients_args);
        ntoh_on_packet_data(&packet_data);

        // Parse the client's address
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf(LOG_INFO"Data received from %s:%d\n", client_ip_str, ntohs(client_address.sin_port));

        if (datagram_is_valid(&packet_data)) {
            printf("Number of pairs: %d\n", packet_data.count);
            printf("Packet id: %d\n", packet_data.id);
            printf("Pairs: \n");
            char printable_key[KEY_SIZE + 1];
            char printable_value[VALUE_SIZE + 1]; 
            printable_key[KEY_SIZE] = '\0';
            printable_value[VALUE_SIZE] = '\0';
            for (int i = 0; i < packet_data.count; i++) {
                memcpy(printable_key, packet_data.pairs[i].key, KEY_SIZE);
                memcpy(printable_value, packet_data.pairs[i].value, VALUE_SIZE);

                printf("%s:%s\n", printable_key, printable_value);
            }
            printf("\n");
        }
        

        response_t response_to_client = {
            (uint16_t) htons(packet_data.id),
            (uint8_t) htons(0)
        };
        message_args_t send_res_to_client_args = {
            sockfd,
            &response_to_client,
            sizeof(response_to_client),
            &client_address
        };
        send_message(&send_res_to_client_args);
    }
    exit(0);
}