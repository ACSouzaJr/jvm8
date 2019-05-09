main: 
	gcc -Wall -g -O0 -std=c99 main.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind:
	valgrind --leak-check=full ./a.out
