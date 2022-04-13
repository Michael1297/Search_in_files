cmake_minimum_required(VERSION 3.21)

add_executable(${PROJECT_NAME}
        main.cpp
        ConverterJSON.cpp
        ${INCLUDE_DIR}/ConverterJSON.h
        ${INCLUDE_DIR}/Exception.h
        ${INCLUDE_DIR}/InvertedIndex.h
        )

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})