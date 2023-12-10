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
#include "serialize.h"
#include "node.h"

int main(int argc, char *argv[]) {
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
    set_values(B, 3, 4, "def\0", 4);
    Node* C = create_node();
    set_values(C, 5, 6, "test\0", 5);
    add_node(B, C);

    print_nodes(B);

    uint8_t buf[1024];
    uint16_t size = pack(buf, B);
    delete_list(B);

//    for(int i = 0; i < size; i++) {
//        printf("i: %d, bajt: %d\n", i, buf[i]);
//    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        LOG_ERROR("connect() failed");
        perror("connect failed");
    }

    if (write(sockfd, buf, size) == -1) {
        LOG_ERROR("writing on stream socket");
        perror("writing on stream socket");
    }

    if (read(sockfd, buf, 100) == -1) {
        LOG_ERROR("reading on stream socket");
        perror("reading on stream socket");
    }

    Node* X = unpack(buf);
    print_nodes(X);

    // Close the socket
    close(sockfd);

    return 0;
}