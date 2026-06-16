cmake_minimum_required(VERSION 3.16)

project(Raccon C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(logging STATIC
    Logging/logger.c
)

target_include_directories(logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Logging
)

add_library(networking STATIC
    Networking/Socket.c
)

target_include_directories(networking
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Logging
        ${CMAKE_CURRENT_SOURCE_DIR}/Networking
)

target_link_libraries(networking
    PUBLIC
        logging
)

add_executable(test 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test.c
)

target_link_libraries(test
    PRIVATE
        networking
)
