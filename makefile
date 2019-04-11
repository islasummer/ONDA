all: main_lite_working.x

main_lite_working.x: main_lite_working.cpp wav_record_function_working.o
	g++ main_lite_working.cpp wav_record_function_working.o -o main_lite_working.x -lasound -std=c++0x

wav_record_function_working.o: wav_record_function_working.cpp wav_header_working.h
	g++ wav_record_function_working.cpp -c -o wav_record_function_working.o -lasound -std=c++0x
