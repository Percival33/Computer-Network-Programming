cmake_minimum_required(VERSION 3.16)
project(Z1.2_C)

set(CMAKE_C_STANDARD 11)

include_directories(../.. include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output)

add_executable(server
    ../../common.c
    src/client_threads_data_t.c
    src/client.c
    src/one_use_socket.c
    src/server.c
)

add_executable(client
    src/client.c
    ../../common.c    
)