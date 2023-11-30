//
// Created by Marcin Jarczewski on 21/11/2023.
//

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

// Error codes
#define OK 0
#define ERROR_INVALID_ARG 1
#define ERROR_FAILED_SOCKET_CREATION 2
#define ERROR_FAILED_SOCKET_BIND 3
#define ERROR_FAILED_TO_RECEIVE_A_MESSAGE 4
#define ERROR_FAILED_TO_SEND_A_MESSAGE 5
#define ERROR_CLIENT_THREAD_BUFFER_TOO_SMALL 6
#define ERROR_UNKNOWN_PACKET_TYPE 7
#define ERROR_INVALID_DATAGRAM 8

// Constants
#define BUFFER_SIZE 512 // Buffer size for data
#define HEADER_SIZE 4 // sizeof id + size
#define MAX_PAYLOAD_SIZE (BUFFER_SIZE - HEADER_SIZE)
#define KEY_SIZE 2
#define VALUE_SIZE 2
#define PAIR_SIZE (KEY_SIZE + VALUE_SIZE)
#define MAX_PAIR_COUNT (MAX_PAYLOAD_SIZE/PAIR_SIZE)
#define RESPONSE_WAIT_TIME_S 1

// Printing
#define RED_TEXT "\033[1;31m"
#define BLUE_TEXT "\033[1;34m"
#define WHITE_TEXT "\033[1;37m"
#define RESET_COLOR "\033[0m"
#define LOG_ERROR RED_TEXT "ERROR: " RESET_COLOR
#define LOG_INFO BLUE_TEXT "INFO: " RESET_COLOR
#define LOG_DEBUG WHITE_TEXT "DEBUG: " RESET_COLOR

#endif