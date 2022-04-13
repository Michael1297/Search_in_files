cmake_minimum_required(VERSION 3.21)
project(search_engine)
set(PROJECT_VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(third_party/json)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)