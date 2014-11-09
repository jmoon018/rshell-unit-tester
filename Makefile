all:
	if[ -d "/bin"]; \
		then mkdir bin; \
	fi
		g++ -Wall -Werror -ansi -pedantic src/exec.cpp -o bin/rshell

rshell:
	g++ -Wall -Werror -ansi -pedantic src/exec.cpp -o bin/rshell

test:
	g++ -Wall -Werror -ansi -pedantic src/autograder.cpp -o autograder
