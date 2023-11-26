cmake_minimum_required(VERSION 3.16)
project(Z1.2_C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

# For a very clean compile
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Werror")

set(SERVER_TARGET server)
set(CLIENT_TARGET client)

include_directories(../.. include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./output)

add_executable(${SERVER_TARGET}
    ../../common.c
    src/client_threads_data_t.c
    src/one_use_socket.c
    src/server.c
)
# target_compile_options(${SERVER_TARGET} PRIVATE -lpthread)
target_link_libraries(${SERVER_TARGET} PRIVATE pthread)

add_executable(${CLIENT_TARGET}
    src/client.c
    ../../common.c    
)
target_link_libraries(${CLIENT_TARGET} PRIVATE pthread)
