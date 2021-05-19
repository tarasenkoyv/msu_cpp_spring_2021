CC=g++
FLAGS=-std=c++17 -Wall -Wextra

all: test

test: pool_test.o
	$(CC) $(FLAGS) pool_test.o -o pool_test
	./pool_test

pool_test.o: pool_test.cpp pool.hpp
	$(CC) $(FLAGS) pool_test.cpp -c
	
clean:
#	del -f *.o pool_test.exe
	rm -f *.o pool_test.exe
