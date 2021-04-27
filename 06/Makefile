CC=g++
FLAGS=-std=c++17 -Wall -Wextra

all: test

test: format_test.o
	$(CC) $(FLAGS) format_test.o -o format_test
	./format_test

format_test.o: format_test.cpp format.hpp
	$(CC) $(FLAGS) format_test.cpp -c
	
clean:
#	del -f *.o format_test.exe
	rm -f *.o format_test.exe
