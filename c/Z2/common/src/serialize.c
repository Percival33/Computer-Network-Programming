#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialize.h"

void packu16(uint8_t* buf, uint16_t i) {
    *buf++ = i >> 8; *buf++ = i;
}

void packu32(uint8_t* buf, uint32_t i) {
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}

uint16_t pack(uint8_t* buf, Node* node) {
    uint8_t* buf_start = buf;
    uint16_t length = 0;
    buf += 2;
    uint16_t size = 2;
    // need to move pointer manually as pack* moves its copy
    for(; node != NULL; node = node->next, length++) {
        size += 2 + 4 + 2 + strlen(node->text);
        packu16(buf, node->a);
        buf += 2;

        packu32(buf, node->b);
        buf += 4;

        uint16_t len = strlen(node->text);
        packu16(buf, len);
        buf += 2;

        for(uint16_t j = 0; j < len; j++) {
            *buf++ = node->text[j];
        }
    }
    packu16(buf_start, length);
    char log_msg[MAX_TEXT_SIZE];
    sprintf(log_msg, "Packed size: %d", size);
    LOG_INFO(log_msg);
    return size;
}

uint16_t unpacku16(uint8_t* buf) {
    uint16_t i = ((uint16_t)buf[0]<<8) | buf[1];
    return i;
}

uint32_t unpacku32(uint8_t* buf) {
    return  ((uint32_t)buf[0]<<24)  |
            ((uint32_t)buf[1]<<16)  |
            ((uint32_t)buf[2]<<8)   |
            buf[3];
}

Node* unpack(uint8_t* buf) {
    Node *root = NULL, *curr_node = NULL, *prev_node = NULL;
    uint16_t str_len;

    uint16_t size = unpacku16(buf);

    buf += 2;

    char log_msg[MAX_TEXT_SIZE];
    sprintf(log_msg, "No of nodes to deserialize: %d", size);
    LOG_INFO(log_msg);

    for(uint16_t i = 0; i < size; i++) {
        curr_node = create_node();
        if(prev_node == NULL) {
            root = curr_node;
        } else {
            prev_node->next = curr_node;
        }
        prev_node = curr_node;

        curr_node->a = unpacku16(buf);
        buf += 2;

        curr_node->b = unpacku32(buf);
        buf += 4;

        str_len = unpacku16(buf);
        buf += 2;

        char* str = (char*)malloc(str_len * sizeof(char));
        for(uint16_t j = 0; j < str_len; j++) {
            str[j] = (unsigned char)*buf++;
        }
        // str_len is counted excluding the null sign!!!
        str[str_len] = '\0';
        assign_text(curr_node, str, str_len + 1);
    }

    return root;
}
