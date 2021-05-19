CC=g++
FLAGS=-std=c++17 -Wall -Wextra

all: test

sort_thread.o: sort_thread.hpp utils.hpp sort_thread.cpp
	$(CC) $(FLAGS) sort_thread.cpp -c
	
commander.o: commander.hpp sort_thread.hpp utils.hpp commander.cpp
	$(CC) $(FLAGS) commander.cpp -c
	
utils.o: utils.hpp utils.cpp
	$(CC) $(FLAGS) utils.cpp -c

filesort_test.o: filesort_test.cpp commander.hpp utils.hpp
	$(CC) $(FLAGS) filesort_test.cpp -c
	
test: filesort_test.o utils.o commander.o sort_thread.o
	$(CC) $(FLAGS) filesort_test.o utils.o commander.o sort_thread.o -o filesort_test
	./filesort_test


	
clean:
#	del -f *.o *.bin filesort_test.exe
	rm -f *.o *.bin filesort_test.exe
