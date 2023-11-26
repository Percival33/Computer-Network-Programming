#include "common.h"

int send_message(message_args_t *args) {
    int data_length = sendto(
            args->sockfd,
            args->message_buffer,
            args->message_buffer_length,
            0,
            (struct sockaddr*) args->to_address,
            (socklen_t) sizeof(*args->to_address)
    );
    if (data_length == -1) {
        perror("Failed to send the message");
        exit(ERROR_FAILED_TO_SEND_A_MESSAGE);
    }
    return data_length;
}

int receive_message(message_args_t *args) {
    int data_length = recvfrom(
        args->sockfd,
        args->message_buffer, 
        args->message_buffer_length,
        0,
        (struct sockaddr*) args->to_address,
        &(socklen_t){sizeof(args->to_address)}
    );
    printf("bbb\n");
    if (data_length == -1) {
        printf("Failed to receive the message\n");
        exit(ERROR_FAILED_TO_RECEIVE_A_MESSAGE);
    }
    return data_length;
}

void *resender(void *args) {
    printf("Resender thread has started.\n");
    resender_args_t *args_parsed = (resender_args_t*) args;
    while (true) {
        send_message(args_parsed->send_message_args);
        sleep(RESPONSE_WAIT_TIME_S);
        if (args_parsed->message_received) {
            printf("Resender thread has ended.\n");
            return NULL;
        }
    }
}
