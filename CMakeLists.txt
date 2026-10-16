cmake_minimum_required(VERSION 4.3)
project(tesserac)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(tesserac
        main.cpp
        lexer.cpp
        parser.cpp
        codeGenerator.cpp
)