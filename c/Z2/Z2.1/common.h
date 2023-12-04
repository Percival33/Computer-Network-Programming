//
// Created by Marcin Jarczewski on 04/12/2023.
//

#ifndef COMPUTER_NETWORK_PROGRAMMING_COMMON_H
#define COMPUTER_NETWORK_PROGRAMMING_COMMON_H

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
#define MAX_TEXT_SIZE 12

#define RED_TEXT "\033[1;31m"
#define BLUE_TEXT "\033[1;34m"
#define WHITE_TEXT "\033[1;37m"
#define RESET_COLOR "\033[0m"

#define LOG_ERROR(message) printf(RED_TEXT "ERROR:\t" RESET_COLOR "%s\n", message)
#define LOG_INFO(message) printf(BLUE_TEXT "INFO:\t" RESET_COLOR "%s\n", message)
#define LOG_DEBUG(message) printf(WHITE_TEXT "DEBUG:\t" RESET_COLOR "%s\n", message)


#endif //COMPUTER_NETWORK_PROGRAMMING_COMMON_H
