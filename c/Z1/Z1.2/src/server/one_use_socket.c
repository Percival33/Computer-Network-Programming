#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "one_use_socket.h"
#include "common.h"
#include "client_threads_data.h"

int create_one_use_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd  < 0) {
        perror("Failed to create a socket");
        exit(ERROR_FAILED_SOCKET_CREATION);
    }

    // int recvBufferSize = 1024 * 1024 * 2; // example buffer size: 2 MB
    // // Set the option
    // if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvBufferSize, sizeof(recvBufferSize)) < 0) {
    //     perror("setsockopt SO_RCVBUF failed");
    //     // Handle error
    // }

    // int reuse = 1;
    // // Set the option
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    //     perror("setsockopt SO_RESUSEADDR failed");
    //     // Handle error
    // }

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = 0; // Random port

    if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind a socket");
        exit(ERROR_FAILED_SOCKET_BIND);
    }

    return sockfd;
}

void one_use_socket_send_message(
    message_contents_t *message_contents,
    struct sockaddr_in *client_address
) {
    int sockfd = create_one_use_socket();
    message_args_t send_message_args = {
        sockfd,
        (void*) message_contents->data,
        message_contents->length,
        client_address
    };
    send_message(&send_message_args);
    close(sockfd);
}

int one_use_socket_receive_message(
    message_contents_t *message_contents,
    struct sockaddr_in *client_address
) {
    int sockfd = create_one_use_socket();
    message_args_t receive_message_args = {
        sockfd,
        (void *) message_contents->data,
        message_contents->length,
        client_address
    };
    int data_length = receive_message(&receive_message_args);
    close(sockfd);
    return data_length;
}

void *one_use_socket_resender(void *args) {
    printf("Resender thread has started.\n");
    client_thread_data_t *client_thread_data = (client_thread_data_t *) args;
    while (true) {
        message_contents_t message_contents = {
            &client_thread_data->response,
            sizeof(client_thread_data->response),
        };
        one_use_socket_send_message(&message_contents, &client_thread_data->id.client_address);
        sleep(RESPONSE_WAIT_TIME_S);
        if (client_thread_data->confirmation_received) {
            printf("Resender thread has ended.\n");
            return NULL;
        }
    }
}