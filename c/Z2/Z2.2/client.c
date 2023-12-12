//
// Created by Marcin Jarczewski and Michał Sobiech on 06/12/2023.
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
#include "check_ip_type.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    // Check the IP type (IPv4 or IPv6)
    int ip_type = check_ip_type(ip);
    if (ip_type == INVALID_IP) {
        printf(LOG_ERROR("Invalid IP.\n"));
        exit(ERROR_INVALID_IP);
    }

    // Server address
    struct sockaddr serverAddr;
    int serverAddrLen;
    memset(&serverAddr, 0, sizeof(serverAddr));

    // Address family
    int address_family;

    // Setup the server address
    if (ip_type == IPV4) {
        address_family = AF_INET;

        ((struct sockaddr_in *) &serverAddr)->sin_family = address_family;
        ((struct sockaddr_in *) &serverAddr)->sin_port = htons(port);
        inet_pton(address_family, ip, (struct sockaddr_in *) &serverAddr);

        serverAddrLen = sizeof(struct sockaddr_in);
    }
    else if (ip_type == IPV6) {
        int address_family = AF_INET6;

        ((struct sockaddr_in6 *) &serverAddr)->sin6_family = address_family;
        ((struct sockaddr_in6 *) &serverAddr)->sin6_port = htons(port);
        inet_pton(address_family, ip, (struct sockaddr_in6 *) &serverAddr);

        serverAddrLen = sizeof(struct sockaddr_in6);
    }

    // Create a socket
    int sockfd = socket(address_family, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Create the data
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

    // if (connect(sockfd, (struct sockaddr*)&serverAddr, serverAddrLen) != 0) {
    //     LOG_ERROR("connect() failed");
    //     perror("connect failed");
    // }

    // if (write(sockfd, buf, size) == -1) {
    //     LOG_ERROR("writing on stream socket");
    //     perror("writing on stream socket");
    // }

    Node* C = unpack(buf);
    print_nodes(C);

    delete_node(B);
    delete_node(C);
    // Close the socket
    close(sockfd);

    return 0;
}