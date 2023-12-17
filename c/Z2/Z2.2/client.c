//
// Created by Michał Sobiech and Marcin Jarczewski on 04/12/2023.
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

#include <netinet/in.h>
#include "common.h"
#include "serialize.h"
#include "node.h"
#include "check_ip_type.h"

int main(int argc, char *argv[]) {
    printf("Z2.2 C client\n");

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

    // Server address length
    int serverAddrLen;

    struct sockaddr_storage serverAddr; // Używamy sockaddr_storage, który obsługuje zarówno IPv4, jak i IPv6
    memset(&serverAddr, 0, sizeof(serverAddr));


    if (ip_type == IPV4) {
        struct sockaddr_in *serverAddr4 = (struct sockaddr_in *)&serverAddr;
        serverAddr4->sin_family = AF_INET;
        serverAddr4->sin_port = htons(port);
        inet_pton(AF_INET, ip, &serverAddr4->sin_addr);
        serverAddrLen = sizeof(struct sockaddr_in);
    }
    else if (ip_type == IPV6) {
        struct sockaddr_in6 *serverAddr6 = (struct sockaddr_in6 *)&serverAddr;
        serverAddr6->sin6_family = AF_INET6;
        serverAddr6->sin6_port = htons(port);
        inet_pton(AF_INET6, ip, &serverAddr6->sin6_addr);
        serverAddrLen = sizeof(struct sockaddr_in6);
    }

    int sockfd;
    if (ip_type == IPV4) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    } else {
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    }

    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Generate a lot of data
    Node* head = create_node();
    set_values(head, 3, 4, "def\0", 4);

    Node* tail = head;
    int nodeCount = 10;
    int minLength = 1;
    char text[minLength + nodeCount + 1];
    for (int i = 0; i < nodeCount; i++) {
        Node* newNode = create_node();

        // Expected text: something like "aaaaaa\0",
        // Each time longer by 1 'a'

        int textLength = minLength + i + 1;
        for (int k = 0; k < textLength - 1; k++) {
            text[k] = 'a';
        }
        text[textLength - 1] = '\0';

        set_values(newNode, 5, 6, text, textLength);
        add_node(tail, newNode);
        tail = newNode;
    }

    print_nodes(head);

    uint8_t buf[1024];
    uint16_t size = pack(buf, head);
    delete_list(head);


    if (connect(sockfd, (struct sockaddr*)&serverAddr, serverAddrLen) != 0) {
        LOG_ERROR("connect() failed");
        perror("connect failed");
    }

    if (write(sockfd, buf, size) == -1) {
        LOG_ERROR("writing on stream socket");
        perror("writing on stream socket");
    }

    // Close the socket
    close(sockfd);

    return 0;
}