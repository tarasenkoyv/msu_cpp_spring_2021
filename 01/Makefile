CC=g++
FLAGS=-std=c++17 -Wall -Wextra

all: test

allocator.o: allocator.hpp allocator.cpp
	$(CC) $(FLAGS) allocator.cpp -c

test: allocator_test.o allocator.o
	$(CC) $(FLAGS) allocator_test.o allocator.o -o allocator_test
	./allocator_test

allocator_test.o: allocator_test.cpp
	$(CC) $(FLAGS) allocator_test.cpp -c

clean:
#	del -f *.o allocator_test.exe
	rm -f *.o allocator_test.exe
