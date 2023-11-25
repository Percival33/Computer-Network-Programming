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
#include "client_threads_data_t.h"
#include "one_use_socket.h"

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

// A function for managing a connection with a client.
// Used in a thread.
// Each client thread is characterised by the 
// client's address and the packet id number.
void handle_client(void *args) {
    client_thread_data_t *parsed_args = (client_thread_data_t *) args;
    while(true) {
        // if (!parsed_args->confirmation_received) {
        //     // The datagram was received 
            
        // }
        // else {

        // }
        ;
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    int port = atoi(argv[1]);

    client_threads_data_t client_threads_data_list;
    init_client_thread_data_list(&client_threads_data_list);
    while(true) {
        // This loop will receive UDP packets
        // and decide what to do with them.

        // 1. Receive

        data_t packet_data;
        struct sockaddr_in client_address;

        data_and_client_address_t data_and_sender = {
            &packet_data,
            &client_address
        };
        one_use_socket_receive_message(port, &data_and_sender);
        
        ntoh_on_packet_data(&packet_data);

        // Parse the client's address
        char client_ip_str[INET_ADDRSTRLEN];
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

        // 2. Decide which thread should the package be forwarded to

        transmission_id_t client_thread_id = {
            &client_address,
            &packet_data.id
        };
        int index = get_client_thread_data_index(&client_threads_data_list, &client_thread_id);
        if (index >= 0) {
            // Thread for this transmission exists.
            // That means this packet is a response from the client,
            // not a datagram.
            // Therefore the transmission ends.
            client_thread_data_t *thread_data = get_client_thread_data(&client_threads_data_list, index);
            thread_data->confirmation_received = true;
        }
        else {
            // This is the datagram, the first message.
            // Create a thread for the transmission.
            response_t response_to_client = {
                (uint16_t) htons(packet_data.id),
                (uint8_t) htons(0)
            };
            client_thread_data_t client_thread_data = {
                (transmission_id_t){
                    client_address,
                    packet_data.id
                },
                response_to_client,
                false
            };
            add_client_thread_data_to_list(&client_threads_data_list, client_thread_data);
            pthread_create(&one_use_socket_resender, NULL, one_use_socket_resender, (void *)&client_thread_data);
        }
    }
    exit(0);
}