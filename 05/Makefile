CC=g++
FLAGS=-std=c++17 -Wall -Werror -Wextra

all: test

test: main_test.o serializer.hpp deserializer.hpp serializable_types.hpp error.hpp
	$(CC) $(FLAGS) main_test.o -o main_test
	./main_test

main_test.o: main_test.cpp
	$(CC) $(FLAGS) main_test.cpp -c

clean:
#	del -f *.o main_test.exe
	rm -f *.o main_test.exe
