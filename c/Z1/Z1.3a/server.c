// Created by Michał Sobiech on 20.11.2023

#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ERROR_INVALID_ARGC 1
#define ERROR_FAILED_SOCKET_CREATION 2
#define ERROR_FAILED_SOCKET_BIND 3
#define ERROR_FAILED_DATA_RECEIVAL 4
#define ERROR_FAILED_TO_SEND_RESPONSE 5

#define PAIR_SIZE 4
#define BUFFER_SIZE (1<<16)

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
    memset(buffer, '\0', BUFFER_SIZE-1);
    char client_ip_str[INET_ADDRSTRLEN];
    while(true) {
        int data_length = recvfrom(
            sockfd, buffer, sizeof(buffer), 0,
            (struct sockaddr*)&client_address,
            &(socklen_t){sizeof(client_address)}
        );
        if (data_length == -1) {
            perror("Failed to receive data");
            exit(ERROR_FAILED_DATA_RECEIVAL);
        }
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip_str, sizeof(client_ip_str));
        printf("Data received from %s:%d. Size: %d\n",
            client_ip_str,
            ntohs(client_address.sin_port),
            data_length
        );
        char response[5];
        sprintf(response, "ok");
        printf("Sending response (network encoded): %s\n", response);
        if (sendto(sockfd, response, sizeof(response), 0,
            (struct sockaddr*) &client_address, 
            sizeof(client_address)) == -1
        ) {
            perror("Failed to send a response");
            exit(ERROR_FAILED_TO_SEND_RESPONSE);
        }
    }

    exit(0);
}