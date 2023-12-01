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
#include "client_threads_data.h"
#include "one_use_socket.h"
#include "datagram.h"
#include "response.h"
#include "packet_utils.h"
#include "socket.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    int port = atoi(argv[1]);

    int sockfd = create_binded_socket(port);

    client_thread_list_t client_threads_data_list;
    init_client_thread_data_list(&client_threads_data_list);
    while(true) {
        // This loop will receive UDP packets
        // and decide what to do with them.

        // 1. Receive

        char data_buffer[BUFFER_SIZE];
        struct sockaddr_in client_address;

        message_args_t message_args = {
            sockfd,
            data_buffer,
            sizeof(data_buffer),
            &client_address
        };
        int received_data_length = receive_message(&message_args);

        // Parse the client's address
        char client_ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf("Data received from %s:%d\n", client_ip_str, ntohs(client_address.sin_port));

        // Debug
        // printf("Thread count: %d\n", get_client_thread_count(&client_threads_data_list));

        // 2. Decide which thread should the package be forwarded to

        int packet_type = return_packet_type(received_data_length);
        if (packet_type == DATAGRAM_PACKET) {
            data_t *datagram = (data_t *) data_buffer;

            // Perform an ntoh function on datagram's numeric fields
            ntoh_on_datagram(datagram);

            // The datagram might be aduplicated one. Check if a thread for it exits:
            transmission_id_t client_thread_id = {
                client_address,
                datagram->id
            };
            int index = get_client_thread_data_index(&client_threads_data_list, &client_thread_id);
            if (index != -1) {
                printf("DUPLICATE!\n");
                continue;
            }

            // The datagram is not duplicated. Proceed.

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
            
            client_thread_data_t *client_thread_data = add_client_thread_data_to_list(
                &client_threads_data_list,
                (client_thread_data_t) {
                    (transmission_id_t){
                        client_address,
                        datagram->id
                    },
                    response_to_client,
                    false
                }
            );
            pthread_t resender_thread;
            pthread_create(&resender_thread, NULL, one_use_socket_resender, (void *)client_thread_data);
        }
        else if (packet_type == RESPONSE_PACKET) {
            response_t *response = (response_t *) data_buffer;

            // Perform an ntoh function on response's numeric fields
            ntoh_on_response(response);

            // The response might be duplicated. Check if it has a transmission.
            transmission_id_t client_thread_id = {
                client_address,
                response->id
            };
            int index = get_client_thread_data_index(&client_threads_data_list, &client_thread_id);
            if (index == -1) {
                printf("DUPLICATE!\n");
                continue;
            }

            // Response belongs to a thread. Proceed.
            
            // The thread for this transmission exists.
            // Find it and inform it, that the transmission has ended
            client_thread_data_t *thread_data = get_client_thread_data(&client_threads_data_list, index);
            
            thread_data->confirmation_received = true;
            remove_client_thread_data_from_list(&client_threads_data_list, index);
            
            int expected_packet_id = thread_data->id.packet_id;
            if (response_is_valid(response, expected_packet_id)) {
                print_response_data(response);
            }
        }
    }
    exit(0);
}