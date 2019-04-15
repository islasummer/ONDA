cmake_minimum_required(VERSION 3.10)
project(ONDA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_subdirectory(record)

add_executable(ONDA
        fftw_test4.cpp
        pcmread.h
        pitchassignclass.h
        pitchdetectclass.h
        displayclass.h
)

target_link_libraries(ONDA fftw3 record wiringPi wiringPiDev)
