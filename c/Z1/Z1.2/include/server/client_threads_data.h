#ifndef CLIENT_THREADS_DATA_H
#define CLIENT_THREADS_DATA_H

#include <arpa/inet.h>
#include "common.h"
#include "message_utils.h"
#include "response.h"

#define MAX_THREADS 512

// A struct containing the data that identifies
// A transmission between server and a client.
// Something similar to a socket.
typedef struct {
    struct sockaddr_in client_address;
    int packet_id;
} transmission_id_t;

typedef struct {
    transmission_id_t id;
    response_t response;
    bool confirmation_received;
} client_thread_data_t;

typedef struct {
    client_thread_data_t data[MAX_THREADS];
    bool data_array_index_occupied[MAX_THREADS];
} client_thread_list_t;

// Returns the first free index in the thread list
int first_free_index(client_thread_list_t *list);

int add_client_thread_data_to_list(
    client_thread_list_t *list,
    client_thread_data_t client_thread_data
);

// Returns the index of the thread characterized by given thread id
int get_client_thread_data_index(
    client_thread_list_t *client_threads_data,
    transmission_id_t *thread_id
);

void init_client_thread_data_list(client_thread_list_t *list);

client_thread_data_t *get_client_thread_data(
    client_thread_list_t *thread_data_list,
    int index
);

#endif