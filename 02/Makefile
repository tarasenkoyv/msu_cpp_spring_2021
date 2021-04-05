CC=g++
FLAGS=-std=c++17 -Wall -Werror -Wextra

all: test

token_parser.o: token_parser.hpp token_parser.cpp
	$(CC) $(FLAGS) token_parser.cpp -c

test: token_parser_test.o token_parser.o
	$(CC) $(FLAGS) token_parser_test.o token_parser.o -o token_parser_test
	./token_parser_test

token_parser_test.o: token_parser_test.cpp
	$(CC) $(FLAGS) token_parser_test.cpp -c

clean:
#	del -f *.o token_parser_test.exe
	rm -f *.o token_parser_test.exe
