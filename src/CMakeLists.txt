cmake_minimum_required(VERSION 3.21)

configure_file(${INCLUDE_DIR}/Version.h.in ${INCLUDE_DIR}/Version.h @ONLY)

add_executable(${PROJECT_NAME}
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        ${INCLUDE_DIR}/ConverterJSON.h
        ${INCLUDE_DIR}/Exception.h
        ${INCLUDE_DIR}/InvertedIndex.h
        ${INCLUDE_DIR}/Version.h
        ${INCLUDE_DIR}/SearchServer.h
        ${INCLUDE_DIR}/Search.hpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})