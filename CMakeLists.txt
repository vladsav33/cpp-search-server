cmake_minimum_required(VERSION 3.16)
project(Dequeue)

set(CMAKE_CXX_STANDARD 17)

add_executable(Dequeue main.cpp document.cpp document.h search_server.cpp search_server.h request_queue.cpp request_queue.h paginator.h string_processing.cpp string_processing.h read_input_functions.cpp read_input_functions.h)