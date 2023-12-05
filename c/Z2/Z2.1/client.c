//
// Created by Marcin Jarczewski on 04/12/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdint.h>
#include <inttypes.h>

#include "common.h"
typedef struct Node Node;

struct Node {
    struct Node* next;
    uint16_t a;
    uint32_t b;
    char* text; // max size MAX_TEXT_SIZE;
};

Node* create_node() {
    Node* new_node = (Node*)malloc(sizeof(Node));
    assert(new_node != NULL);

    new_node->next = NULL;
    new_node->a = 0;
    new_node->b = 0;
//    new_node->text = NULL;
    new_node->text = (char*)malloc(5 * sizeof(char));
    strncpy(new_node->text, "abc\0", 4);
    return new_node;
}

void print_nodes(Node* node) {
    while(node != NULL) {
        printf("%d %d %s\n", node->a, node->b, node->text);
        node = node->next;
    }
}

void assign_text(Node* node, char* new_text, int len) {
    if(node->text != NULL) {
        free(node->text);
    }

    node->text = (char*)malloc(len * sizeof(char));
    strncpy(node->text, new_text, len);
}

void add_node(Node* root, Node *new_node) {
    assert(root != NULL);

    while(root->next != NULL) root = root->next;
    root->next = new_node;
}

void delete_node(Node* node) {
    if(node == NULL)
        return;

    if(node->text != NULL)
        free(node->text);

    free(node);
}

void delete_list(Node* last) {
    Node* prev;
    while(last->next != NULL) {
        prev = last;
        last = last->next;
        delete_node(prev);
    }
}

int length(Node* root) {
    int ret = 0;
    while (root != NULL) {
        root = root->next;
        ret++;
    }
    return ret;
}

Node* prepare_data(Node* root) {
    assert(root != NULL);
    char* text = (char*)malloc(10 * sizeof(char));
    strncpy(text, "testVal", 7);
//    add_node(root, 1, 1, text);
    LOG_INFO("added new node");
    return root;
}

void packu16(uint8_t* buf, uint16_t i) {
//    i = htons(i);
    *buf++ = i >> 8; *buf++ = i;
}

void packu32(uint8_t* buf, uint32_t i) {
//    i = htonl(i);
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}

uint16_t pack(uint8_t* buf, Node* node) {
    packu16(buf, length(node));
    buf += 2;
    uint16_t size = 2;
    // need to move pointer manually as pack* moves its copy
    for(; node != NULL; node = node->next) {
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
    char log_msg[MAX_TEXT_SIZE];
    sprintf(log_msg, "Packed size: %d", size);
    LOG_INFO(log_msg);
    return size;
}

uint16_t unpacku16(uint8_t* buf) {
    uint16_t i = ((uint16_t)buf[0]<<8) | buf[1];
    printf("unpack: %d\tntohs: %d\n",i, ntohs(i));
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
            prev_node = curr_node;
        } else {
            prev_node->next = curr_node;
        }

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
        str[str_len] = '\0';
        assign_text(curr_node, str, str_len);
    }

    return root;
}

int main(int argc, char *argv[]) {
    /* TODO:
     * create a server
     * */
    if (argc != 3) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    // Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    Node* B = create_node();
    B->a = 3;
    B->b = 4;
    strncpy(B->text, "def\0", 4);
    print_nodes(B);
    uint8_t buf[1024];
    uint16_t size = pack(buf, B);

    for(int i = 0; i < size; i++) {
        printf("i: %d, bajt: %d\n", i, buf[i]);
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        LOG_ERROR("connect() failed");
        perror("connect failed");
    }

    if (write(sockfd, buf, size) == -1) {
        LOG_ERROR("writing on stream socket");
        perror("writing on stream socket");
    }

    Node* C = unpack(buf);
    print_nodes(C);

    delete_node(B);
    delete_node(C);
    // Close the socket
    close(sockfd);

    return 0;
}