CC=g++
FLAGS=-std=c++17 -Wall -Werror -Wextra

all: test

matrix.o: matrix.hpp matrix.cpp
	$(CC) $(FLAGS) matrix.cpp -c

test: matrix_test.o matrix.o
	$(CC) $(FLAGS) matrix_test.o matrix.o -o matrix_test
	./matrix_test

matrix_test.o: matrix_test.cpp
	$(CC) $(FLAGS) matrix_test.cpp -c

clean:
#	del -f *.o matrix_test.exe
	rm -f *.o matrix_test.exe
