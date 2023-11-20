// Created by Michał Sobiech on 20.11.2023

#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ERROR_INVALID_ARGC 1
#define ERROR_FAILED_SOCKET_CREATION 2
#define ERROR_FAILED_SOCKET_BIND 3
#define ERROR_FAILED_DATA_RECEIVAL 4
#define ERROR_FAILED_TO_SEND_RESPONSE 5

#define PAIR_SIZE 4
#define BUFFER_SIZE 1024

#define RESPONSE_WAIT_TIME_S 1

void send_response(int sockfd, char *response, int response_length, 
                   struct sockaddr_in *client_address) {
    if (sendto(sockfd, response, response_length, 0, (struct sockaddr*) client_address,
            sizeof(*client_address)) == -1) {
        perror("Failed to send a response");
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
        int data_length = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &(socklen_t){sizeof(client_address)});
        if (data_length == -1) {
            perror("Failed to receive data");
            exit(ERROR_FAILED_DATA_RECEIVAL);
        }
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

        // int send_response(char *response, int response_length, )
        char response[] = "test";
        send_response(sockfd, response, sizeof(response), &client_address);

        // Wait for the response to the response
        float start_tick = clock();
        float current_tick;
        while(true) {
            int data_length = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr*)&client_address, &(socklen_t){sizeof(client_address)});
            if (data_length != -1) {
                break;
            }
            // Still no response
            current_tick = clock();
            float elapsed_time = (current_tick - start_tick)/CLOCKS_PER_SEC;
            if (elapsed_time > RESPONSE_WAIT_TIME_S) {
                start_tick = current_tick;
                send_response(sockfd, response, sizeof(response),
                        &client_address);
            }
        }
    }
    exit(0);
}