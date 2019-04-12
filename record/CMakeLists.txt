cmake_minimum_required(VERSION 3.7)

add_library(record wav_header_working.h wav_record_function_working.cpp)
target_link_libraries(record asound)
