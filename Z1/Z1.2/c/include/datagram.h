#ifndef DATAGRAM_H
#define DATAGRAM_H

#include "common.h"

#define DATAGRAM_PACKET 0
#define RESPONSE_PACKET 1

typedef struct {
    uint16_t id;
    uint16_t count;
    key_value_pair_t pairs[MAX_PAYLOAD_SIZE];
} data_t;

bool datagram_is_valid(data_t *datagram);

void print_datagram_data(data_t *datagram);

void ntoh_on_datagram(data_t *datagram);

#endif