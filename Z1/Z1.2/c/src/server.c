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


#define DATAGRAM_PACKET 0
#define RESPONSE_PACKET 1


bool text_is_only_zeroes(char *text, int text_length) {
    for (int i = 0; i < text_length; i++) {
        if (text[i] != '\0') {
            return false;
        }
    }
    return true;
}


bool datagram_is_valid(data_t *datagram) {
    // Datagram is valid, if:
    // - After the declared pairs there are only zeroes,
    // - The number of pairs does not exceed the max number of
    // pairs that can fit in a packet
    
    // TODO parametrize

    int count = datagram->count;
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
        key = datagram->pairs[i].key;
        if (!text_is_only_zeroes(key, KEY_SIZE)) {
            printf(LOG_ERROR"Datagram is not valid. Error parsing key[%d]\n", i);
            return false;
        }
        value = datagram->pairs[i].value;
        if (!text_is_only_zeroes(value, VALUE_SIZE)) {
            printf(LOG_ERROR"Datagram is not valid. Error parsing value[%d]=(%s)\n", i, value);
            return false;
        }
    }
    return true;
}


bool response_is_valid(response_t *response, int expected_id) {
    // TODO parametrize
    int packet_id = response->id;
    if (packet_id != expected_id) {
        printf(LOG_ERROR"parsing datagram id. Got (%d) expected(%d)\n", packet_id, expected_id);
        return false;
    }
    int status_code = response->status;
    if (status_code != 0) {
        printf(LOG_ERROR"status. Got (%d) expected(%d)\n", status_code, 0);
        return false;
    }
    return true;
}


// Returns what the packet is: a datagram or a response
int return_packet_type(int data_length) {
    if (data_length == BUFFER_SIZE) {
        return DATAGRAM_PACKET;
    }
    else if (data_length == 4) {
        return RESPONSE_PACKET;
    }
    printf("Unknown packet type, length: %d\n", data_length);
    exit(ERROR_UNKNOWN_PACKET_TYPE);
}


void print_datagram_data(data_t *datagram) {
    printf("Number of pairs: %d\n", datagram->count);
    printf("Packet id: %d\n", datagram->id);
    printf("Pairs: \n");
    char printable_key[KEY_SIZE + 1];
    char printable_value[VALUE_SIZE + 1]; 
    printable_key[KEY_SIZE] = '\0';
    printable_value[VALUE_SIZE] = '\0';
    for (int i = 0; i < datagram->count; i++) {
        memcpy(printable_key, datagram->pairs[i].key, KEY_SIZE);
        memcpy(printable_value, datagram->pairs[i].value, VALUE_SIZE);

        printf("%s:%s\n", printable_key, printable_value);
    }
    printf("\n");
}


void print_response_data(response_t *response) {
    printf("Response's packet id: %d\n", response->id);
    printf("Response status: %d\n", response->status);
}


void ntoh_on_datagram(data_t *datagram) {
    datagram->count = ntohs(datagram->count);
    datagram->id = ntohs(datagram->id);
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

    client_thread_list_t client_threads_data_list;
    init_client_thread_data_list(&client_threads_data_list);
    while(true) {
        // This loop will receive UDP packets
        // and decide what to do with them.

        // 1. Receive

        char data_buffer[BUFFER_SIZE];
        struct sockaddr_in client_address;

        message_contents_t message_contents = {
            data_buffer,
            BUFFER_SIZE
        };
        int received_data_length = one_use_socket_receive_message(port, &message_contents, &client_address);
        
        // ntoh_on_datagram(&datagram);

        // Parse the client's address
        char client_ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf(LOG_INFO"Data received from %s:%d\n", client_ip_str, ntohs(client_address.sin_port));

        // 2. Decide which thread should the package be forwarded to

        int packet_type = return_packet_type(received_data_length);
        if (packet_type == DATAGRAM_PACKET) {
            data_t *datagram = (data_t *) data_buffer;

            // Perform an ntoh function on datagram's numeric fields
            ntoh_on_datagram(datagram);

            if (datagram_is_valid(datagram)) {
                print_datagram_data(datagram);
            }

            // The datagram is the first packet in a transmission.
            // Make a new transmission handling thread.
            
            // The thread will send a response:
            response_t response_to_client = {
                (uint16_t) htons(datagram->id),
                (uint8_t) htons(0)
            };
            client_thread_data_t client_thread_data = {
                (transmission_id_t){
                    client_address,
                    datagram->id
                },
                response_to_client,
                false
            };
            add_client_thread_data_to_list(&client_threads_data_list, client_thread_data);
            pthread_t resender_thread;
            pthread_create(&resender_thread, NULL, one_use_socket_resender, (void *)&client_thread_data);
        }
        else if (packet_type == RESPONSE_PACKET) {
            response_t *response = (response_t *) data_buffer;

            // Perform an ntoh function on response's numeric fields
            ntoh_on_response(response);

            // The thread for this transmission exists.
            // Find it and inform it, that the transmission has ended
            transmission_id_t client_thread_id = {
                client_address,
                response->id
            };
            int index = get_client_thread_data_index(&client_threads_data_list, &client_thread_id);
            client_thread_data_t *thread_data = get_client_thread_data(&client_threads_data_list, index);
            thread_data->confirmation_received = true;
            
            int expected_packet_id = thread_data->id.packet_id;
            if (response_is_valid(response, expected_packet_id)) {
                print_response_data(response);
            }
        }
    }
    exit(0);
}