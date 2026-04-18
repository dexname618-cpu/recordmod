cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

project(RecordMod VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK not set")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
target_link_libraries(${PROJECT_NAME} geode-sdk)