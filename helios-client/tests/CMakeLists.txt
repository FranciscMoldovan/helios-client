cmake_minimum_required(VERSION 3.1)

project(tests)

add_subdirectory(unittests)
add_subdirectory(integrationtests)

include(${CMAKE_SCRIPTS}/compiler_options.cmake REQUIRED)
include(${CMAKE_SCRIPTS}/qt_configuration.cmake REQUIRED)

add_library(${PROJECT_NAME}
    "inc/testapplication.h"
    "src/testapplication.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(${PROJECT_NAME} SYSTEM
    PRIVATE $ENV{GTEST_DIR}/include
)

find_library(GTEST_LIB gtest $ENV{GTEST_DIR}/lib)
find_library(GMOCK_LIB gmock $ENV{GTEST_DIR}/lib)

target_link_libraries(${PROJECT_NAME}
    ${QT5_LIBS}
    ${GTEST_LIB}
    ${GMOCK_LIB}
)
