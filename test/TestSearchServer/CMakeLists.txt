cmake_minimum_required(VERSION 3.14)
project(TestSearchServer)

set(CMAKE_CXX_STANDARD 11)

message(${INCLUDE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(TestSearchServer
        TestSearchServer.cpp
        ${INCLUDE_DIR}/InvertedIndex.h
        ${SOURCE_DIR}/InvertedIndex.cpp
        ${INCLUDE_DIR}/SearchServer.h
        ${SOURCE_DIR}/SearchServer.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

target_link_libraries(TestSearchServer PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(TestSearchServer)