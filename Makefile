all:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/exec.cpp -o bin/rshell
	g++ -Wall -Werror -ansi -pedantic src/autograder.cpp -o bin/autograder

rshell:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/exec.cpp -o bin/rshell

test:
	g++ -Wall -Werror -ansi -pedantic src/autograder.cpp -o bin/autograder
