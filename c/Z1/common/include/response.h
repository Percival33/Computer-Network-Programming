#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t id;
    uint8_t status;
} response_t;

bool response_is_valid(response_t *response, int expected_id);

void print_response_data(response_t *response);

void ntoh_on_response(response_t *response);

#endif