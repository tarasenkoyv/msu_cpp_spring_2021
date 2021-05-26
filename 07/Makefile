CC=g++
FLAGS=-std=c++17 -Wall -Wextra

all: test

test: my_vector_test.o
	$(CC) $(FLAGS) my_vector_test.o -o my_vector_test
	./my_vector_test

my_vector_test.o: my_vector_test.cpp my_vector.hpp allocator.hpp iterator.hpp data_test.hpp
	$(CC) $(FLAGS) my_vector_test.cpp -c
	
clean:
#	del -f *.o my_vector_test.exe
	rm -f *.o my_vector_test
