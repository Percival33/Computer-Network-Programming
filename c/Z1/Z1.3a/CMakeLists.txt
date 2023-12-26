cmake_minimum_required(VERSION 3.16)
project(Z1.3a_C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output)

add_executable(client client.c)
target_link_libraries(client PRIVATE pthread)

add_executable(server server.c)
target_link_libraries(server PRIVATE pthread)
