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

#define DATA_SIZE_KB 1

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
    int sndbufSizekB = atoi(argv[3]);

    // Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(ERROR_FAILED_SOCKET_CREATION);
    }

    // Change the sender buffer size
    int sndbufSize = sndbufSizekB * KB;
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndbufSize, sizeof(sndbufSize)) != 0) {
        perror("setsockopt SO_SNDBUF failed");
        exit(ERROR_SO_SNDBUF_FAILED);
    }

    // Server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    // Generate a lot of data
    uint8_t data_buf[DATA_SIZE_KB * KB];
    for (int i = 0; i < sizeof(data_buf); i++) {
        data_buf[i] = (uint8_t) 'a';
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        LOG_ERROR("connect() failed");
        perror("connect failed");
    }

    int msg_counter = 1;
    while (1) {
        gettimeofday(&start, NULL);
        if (write(sockfd, data_buf, sizeof(data_buf)) == -1) {
            LOG_ERROR("writing on stream socket");
            perror("writing on stream socket");
        }

        gettimeofday(&end, NULL);

        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        total_time = ((seconds) * 1000 + useconds/1000.0) + 0.5;

        printf("Msg no: %d, time between send and ack: %f ms\n", msg_counter, total_time);

        // usleep(10 * 1000);

        msg_counter++;
    }

    // Close the socket
    close(sockfd);

    return 0;
}