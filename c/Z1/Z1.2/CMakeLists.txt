cmake_minimum_required(VERSION 3.16)
project(Z1.2_C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

# For a very clean compile
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Werror")

set(SERVER_TARGET server)
set(CLIENT_TARGET client)

set(C_Z1_FOLDER_DIR ../../Z1)

include_directories(
    ${C_Z1_FOLDER_DIR}/common/include
    include
    include/server
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output)

set(COMMON_SOURCES
    ${C_Z1_FOLDER_DIR}/common/src/message_utils.c
    ${C_Z1_FOLDER_DIR}/common/src/datagram.c
    ${C_Z1_FOLDER_DIR}/common/src/print_address.c
    ${C_Z1_FOLDER_DIR}/common/src/response.c
    ${C_Z1_FOLDER_DIR}/common/src/socket.c
)

add_executable(${SERVER_TARGET}
    src/server/client_threads_data.c
    src/server/one_use_socket.c
    src/server/packet_utils.c
    src/server/server.c
    ${COMMON_SOURCES}
)
target_link_libraries(${SERVER_TARGET} PRIVATE pthread)

add_executable(${CLIENT_TARGET}
    src/client/client.c
    ${COMMON_SOURCES}
)
target_link_libraries(${CLIENT_TARGET} PRIVATE pthread)
