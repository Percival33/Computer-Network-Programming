cmake_minimum_required(VERSION 3.16)
project(Z1.2_C)

set(CMAKE_C_STANDARD 11)

include_directories(../..)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output)

add_executable(server
    src/server.c
    ../../common.c    
)

add_executable(client
    src/client.c
    ../../common.c    
)