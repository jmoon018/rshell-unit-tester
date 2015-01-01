rshellUnitTester:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/rshellUnitTester.cpp -o bin/shelltester
