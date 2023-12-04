//
// Created by Marcin Jarczewski on 04/12/2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "common.h"

typedef struct {
    Node* next;
    uint16_t a;
    uint32_t b;
    char* text; // max size = 10;
} Node;

void add_node(Node* root, uint16_t a, uint32_t b, char* text) {
    while(root->next != NULL) root = root->next;
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->a = a;
    new_node->b = b;
    new_node->text = text;
    assert(strlen(text) < 10);

    strncpy(new_node->text, text, strlen(text));
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
    char* text = (char*)malloc(10 * sizeof(char));
    strncpy(text, "testVal", 7);
    add_node(root, 1, 1, text);
    LOG_INFO("added new node");
    return root;
}

int main(int argc, char *argv[]) {
    /* TODO:
     * convert to tcp
     * send whole list
     * create a server
     * */
    if (argc != 3) {
        printf("Invalid argument count\n");
        exit(ERROR_INVALID_ARG);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    // Socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    // Prepare the data
    Node* root = prepare_data(root);

    // Prepare message


    // Send message
    message_args_t message = {
            sockfd,
            (void*) &data,
            sizeof(data),
            &serverAddr
    };
    send_message(&message);
    LOG_INFO("Message was sent\n");

    // Close the socket
    close(sockfd);
    return 0;

    return 0;
}