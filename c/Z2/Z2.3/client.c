//
// Created by Marcin Jarczewski and Michał Sobiech on 04/12/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdint.h>

#include "common.h"
#include "serialize.h"
#include "node.h"

int main(int argc, char *argv[]) {
    printf("Z2.3a C client\n");

    if (argc != 4) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    struct timeval start, end;
    long seconds, useconds;
    double total_time;

    struct sockaddr_in serverAddr;
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int bufferSize = atoi(argv[3]);

    // Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(ERROR_FAILED_SOCKET_CREATION);
    }

    // Change the sender buffer size
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize)) != 0) {
        perror("setsockopt SO_SNDBUF failed");
        exit(ERROR_SO_SNDBUF_FAILED);
    }

    // Server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    Node* head = create_node();
    set_values(head, 3, 4, "def\0", 4);

    // Generate a lot of data
    Node* tail = head;
    for (int i = 0; i < 1000; i++) {
        Node* newNode = create_node();
        set_values(newNode, 5, 6, "test\0", 5);
        add_node(tail, newNode);
        tail = newNode;
    }

    print_nodes(head);

    uint8_t buf[1024];
    uint16_t size = pack(buf, head);
    delete_list(head);

//    for(int i = 0; i < size; i++) {
//        printf("i: %d, bajt: %d\n", i, buf[i]);
//    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        LOG_ERROR("connect() failed");
        perror("connect failed");
    }
    gettimeofday(&start, NULL);
    if (write(sockfd, buf, size) == -1) {
        LOG_ERROR("writing on stream socket");
        perror("writing on stream socket");
    }
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    total_time = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    printf("Time taken for data send: %f milliseconds\n", total_time);

    if (read(sockfd, buf, 100) == -1) {
        LOG_ERROR("reading on stream socket");
        perror("reading on stream socket");
    }
    printf("Received data: %s\n", buf);

//    Node* X = unpack(buf);
//    print_nodes(NULL);

    // Close the socket
    close(sockfd);

    return 0;
}