//
// Created by Michał Sobiech on 30.11.2023
//


#ifndef PACKET_UTILS_H
#define PACKET_UTILS_H

#define DATAGRAM_PACKET 0
#define RESPONSE_PACKET 1

// Returns what the packet is: a datagram or a response
int return_packet_type(int data_length);

#endif