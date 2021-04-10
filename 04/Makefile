CC=g++
FLAGS=-std=c++17 -Wall -Werror -Wextra

all: test

bigint.o: bigint.hpp bigint.cpp
	$(CC) $(FLAGS) bigint.cpp -c

test: bigint_test.o bigint.o
	$(CC) $(FLAGS) bigint_test.o bigint.o -o bigint_test
	./bigint_test

bigint_test.o: bigint_test.cpp
	$(CC) $(FLAGS) bigint_test.cpp -c

clean:
#	del -f *.o bigint_test.exe
	rm -f *.o bigint_test.exe
