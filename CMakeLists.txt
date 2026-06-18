cmake_minimum_required(VERSION 3.16)

project(Raccon C)

add_compile_definitions(_GNU_SOURCE)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(logging STATIC
    lib/Logging/logger.c
)

target_include_directories(logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Logging
)

add_library(networking STATIC
    lib/Networking/socket.c
    lib/Networking/conn.c
)

target_include_directories(networking
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Logging
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Networking
)

add_library(eventloop STATIC 
    Eventloop/evl.c
)

target_include_directories(eventloop
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Eventloop
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Networking
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Logging
)

target_link_libraries(networking
    PUBLIC
        logging
)

target_link_libraries(eventloop
    PUBLIC
        networking
)

add_executable(test 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test.c
)

target_include_directories(test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Networking
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Logging
        ${CMAKE_CURRENT_SOURCE_DIR}/Eventloop
)

target_link_libraries(test
    PRIVATE
        networking
        eventloop
)
