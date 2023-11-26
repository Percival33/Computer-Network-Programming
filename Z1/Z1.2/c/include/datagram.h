#ifndef DATAGRAM_H
#define DATAGRAM_H

#include "common.h"

typedef struct {
    uint16_t id;
    uint16_t count;
    key_value_pair_t pairs[MAX_PAYLOAD_SIZE];
} data_t;

bool text_is_only_zeroes(char *text, int text_length);

bool datagram_is_valid(data_t *datagram);

void print_datagram_data(data_t *datagram);

void ntoh_on_datagram(data_t *datagram);

#endif