all: main_lite.x

main_lite.x: main_lite.cpp wav_record_function.o
	g++ main_lite.cpp wav_record_function.o -o main_lite.x -lasound -std=c++0x

wav_record_function.o: wav_record_function.cpp wav_header.h
	g++ wav_record_function.cpp -c -o wav_record_function.o -lasound -std=c++0x
