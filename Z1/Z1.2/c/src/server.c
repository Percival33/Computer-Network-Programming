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

#define MAX_THREADS 512

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

// A struct containing the data that identifies
// A transmission between server and a client
typedef struct {
    struct sockaddr_in client_address;
    int packet_id;
} transmission_id_t;

typedef struct {
    transmission_id_t id;
    void *message;
    int message_lenght;
    bool confirmation_received;
} client_thread_data_t;

client_thread_data_t *get_client_thread_data(
        client_threads_data_t *thread_data_list,
        int index) {
    return &thread_data_list->data[index];
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

typedef struct {
    data_t *data;
    struct sockaddr_in *client_address;
} data_and_client_address_t;

// Create a one time socket to send a packet
void one_time_socket_send_message(int port, data_and_client_address_t *data_and_client_address){
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

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind a socket");
        exit(ERROR_FAILED_SOCKET_BIND);
    }
    char client_ip_str[INET_ADDRSTRLEN];

    message_args_t receive_msg_from_clients_args = {
        sockfd,
        (void*) data_and_sender->data,
        sizeof(data_and_sender->data),
        &data_and_sender->client_address
    };
    receive_message(&receive_msg_from_clients_args);
    close(sockfd);
}

// Create a one time socket to receive a packet
void one_time_socket_receive_msg(int port, data_and_client_address_t *data_and_sender) {
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

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind a socket");
        exit(ERROR_FAILED_SOCKET_BIND);
    }
    char client_ip_str[INET_ADDRSTRLEN];

    message_args_t receive_msg_from_clients_args = {
        sockfd,
        (void*) data_and_sender->data,
        sizeof(data_and_sender->data),
        &data_and_sender->client_address
    };
    receive_message(&receive_msg_from_clients_args);
    close(sockfd);
}

// Return the index of the first free index in the
// client_threads array of client_threads_data_t
int first_free_index(client_threads_data_t *list) {
    for (int i = 0; i < sizeof(list->data); i++) {
        if (!list->data_array_index_occupied[i]) {
            return i;
        }
    }
    return -1;
}

int add_client_thread_data_to_list(
        client_threads_data_t *list,
        client_thread_data_t client_thread_data) {
    int index = first_free_index(list);
    if (index == -1) {
        printf(LOG_ERROR"Client thread buffer is too small.\n");
        exit(ERROR_CLIENT_THREAD_BUFFER_TOO_SMALL);
    }
    list->data[index] = client_thread_data;
    list->data_array_index_occupied[index] = true;
    return index;
}

// Returns the index of the thread
// characterized by given thread id
int get_client_thread_data_index(
        client_threads_data_t *client_threads_data,
        transmission_id_t *thread_id) {
    for (int i = 0; i < sizeof(client_threads_data->data_array_index_occupied); i++) {
        if (client_threads_data->data_array_index_occupied[i]) {
            // Compare thread id's
            transmission_id_t i_thread_id = client_threads_data->data[i].id;
            // First the datagram id
            if (!thread_id->packet_id == i_thread_id.packet_id) {
                continue;
            }
            // Compare client IPs
            if (!thread_id->client_address.sin_addr.s_addr == i_thread_id.client_address.sin_addr.s_addr) {
                continue;
            }
            // Compare client ports
            if (!thread_id->client_address.sin_port == i_thread_id.client_address.sin_port) {
                continue;
            }
            return i;
        }
    }
    return -1;
}

void init_client_thread_data_list(client_threads_data_t *list) {
    for (int i = 0; i < sizeof(list->data_array_index_occupied); i++) {
        list->data_array_index_occupied[i] = false;
    }
}



typedef struct {
    client_thread_data_t data[MAX_THREADS];
    bool data_array_index_occupied[MAX_THREADS];
} client_threads_data_t;

void *one_time_socket_resender(void *args) {
    printf("Resender thread has started.\n");
    client_thread_data_t *args_parsed = (client_thread_data_t*) args;
    while (true) {
        one_time_socket_send_message(args_parsed->);
        sleep(RESPONSE_WAIT_TIME_S);
        if (args_parsed->confirmation_received) {
            printf("Resender thread has ended.\n");
            return NULL;
        }
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
        one_time_socket_receive_msg(port, &data_and_sender);
        
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
            client_thread_data_t thread_data = {
                (transmission_id_t){
                    client_address,
                    packet_data.id
                },
                response_to_client,
                sizeof(response_to_client)
                false
            };
            add_client_thread_data_to_list(&client_threads_data_list, thread_data);

            response_t response_to_client = {
                (uint16_t) htons(packet_data.id),
                (uint8_t) htons(0)
            };
            pthread_create(&resender_thread, NULL, one_time_socket_resender, (void *)&resender_args);
        }
    }
    exit(0);
}