//
// Created by Michał Sobiech on 30.11.2023
//


#include "client_threads_data.h"

int first_free_index(client_thread_list_t *list) {
    for (size_t i = 0; i < sizeof(list->data); i++) {
        if (!list->data_array_index_occupied[i]) {
            return i;
        }
    }
    return -1;
}

client_thread_data_t *add_client_thread_data_to_list(
        client_thread_list_t *list,
        client_thread_data_t client_thread_data) {
    int index = first_free_index(list);
    if (index == -1) {
        printf(LOG_ERROR"Client thread buffer is too small.\n");
        exit(ERROR_CLIENT_THREAD_BUFFER_TOO_SMALL);
    }
    list->data[index] = client_thread_data;
    list->data_array_index_occupied[index] = true;
    return &list->data[index];
}

int get_client_thread_data_index(
        client_thread_list_t *client_threads_data,
        transmission_id_t *thread_id) {
    for (size_t i = 0; i < sizeof(client_threads_data->data_array_index_occupied); i++) {
        if (client_threads_data->data_array_index_occupied[i]) {
            // Compare thread id's
            transmission_id_t i_thread_id = client_threads_data->data[i].id;
            // First the datagram id
            if (!(thread_id->packet_id == i_thread_id.packet_id)) {
                printf("ID\n");
                continue;
            }
            // Compare client IPs
            if (!(thread_id->client_address.sin_addr.s_addr == i_thread_id.client_address.sin_addr.s_addr)) {
                printf("IP\n");
                continue;
            }
            // Compare client ports
            // if (!(thread_id->client_address.sin_port == i_thread_id.client_address.sin_port)) {
            //     printf("PORT: %d, %d\n", thread_id->client_address.sin_port, i_thread_id.client_address.sin_port);
            //     continue;
            // }
            return i;
        }
    }
    return -1;
}

void init_client_thread_data_list(client_thread_list_t *list) {
    for (size_t i = 0; i < sizeof(list->data_array_index_occupied); i++) {
        list->data_array_index_occupied[i] = false;
    }
}

client_thread_data_t *get_client_thread_data(
        client_thread_list_t *thread_data_list,
        int index) {
    return &thread_data_list->data[index];
}

void remove_client_thread_data_from_list(
    client_thread_list_t *list,
    int index
) {
    list->data_array_index_occupied[index] = false;
}

int get_client_thread_count(client_thread_list_t *client_threads_data_list) {
    int sum = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (client_threads_data_list->data_array_index_occupied[i]) {
            sum++;
        }
    }
    return sum;
}
