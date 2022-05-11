cmake_minimum_required(VERSION 3.21)
project(search_engine)
set(PROJECT_VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(third_party/json)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

configure_file(config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)
configure_file(requests.json ${CMAKE_CURRENT_BINARY_DIR}/requests.json COPYONLY)
configure_file(answers.json ${CMAKE_CURRENT_BINARY_DIR}/answers.json COPYONLY)