//
// Created by Marcin Jarczewski on 05/12/2023.
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
    /* TODO:
     * create a server
     * modify cmakelists
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

    uint8_t buf[1024];

    // if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
    //     LOG_ERROR("connect() failed");
    //     perror("connect failed");
    // }

    while(1) {
        // TODO: handle connection
        if (read(sockfd, buf, size) == -1) {
            LOG_ERROR("reading on stream socket");
            perror("writing on stream socket");
        }
        Node* A = unpack(buf);
        print_nodes(A);
        delete_node(A);
    }    

    // Close the socket
    close(sockfd);

    return 0;
}