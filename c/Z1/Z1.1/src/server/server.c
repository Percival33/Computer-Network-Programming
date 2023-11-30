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
#include "response.h"
#include "message_utils.h"
#include "socket.h"
#include "datagram.h"
#include "print_address.h"
#include "socket.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    int port = atoi(argv[1]);
    int sockfd = create_binded_socket(port);

    while(true) {
        struct sockaddr_in client_address;

        // Receive data
        data_t packet_data;
        message_args_t receive_msg_from_clients_args = {
            sockfd,
            &packet_data,
            sizeof(packet_data),
            &client_address
        };
        receive_message(&receive_msg_from_clients_args);

        // Fix the byte order
        ntoh_on_datagram(&packet_data);

        // Print the client's address
        char client_ip_str[INET_ADDRSTRLEN];
        print_address(&client_address, client_ip_str, sizeof(client_ip_str));

        // Print the datagram
        if (datagram_is_valid(&packet_data)) {
            print_datagram_data(&packet_data);
        }

        // Send a response
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