cmake_minimum_required(VERSION 3.16)
project(Z2.2_C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

#set(SERVER_TARGET server)
set(CLIENT_TARGET client)

set(C_Z2_FOLDER_DIR ../)

include_directories(
    ${C_Z2_FOLDER_DIR}/common/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output)

set(COMMON_SOURCES
    ${C_Z2_FOLDER_DIR}/common/src/node.c
    ${C_Z2_FOLDER_DIR}/common/src/serialize.c
)

#add_executable(${SERVER_TARGET}
#    src/server/server.c
#    ${COMMON_SOURCES}
#)

add_executable(${CLIENT_TARGET}
    client.c
    ${COMMON_SOURCES}
)
