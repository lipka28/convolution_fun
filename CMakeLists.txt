cmake_minimum_required(VERSION 3.0.0)
project(convolution VERSION 0.1.0)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

#C++14
set(CMAKE_CXX_STANDARD 14)

#folder structure
file(GLOB SOURCES
    headers/*.hpp
    src/*.cpp)

add_executable(convolution ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(convolution ${PNG_LIBRARY})
TARGET_LINK_LIBRARIES(convolution pthread)
