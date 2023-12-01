//
// Created by Michał Sobiech on 30.11.2023
//


#include "packet_utils.h"
#include "common.h"


int return_packet_type(int data_length) {
    if (data_length == BUFFER_SIZE) {
        return DATAGRAM_PACKET;
    }
    else if (data_length == 4) {
        return RESPONSE_PACKET;
    }
    printf("Unknown packet type, length: %d\n", data_length);
    exit(ERROR_UNKNOWN_PACKET_TYPE);
}