main: 
	gcc -Wall -g -O0 -std=c99 main.c op_codes_initializer.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind:
	valgrind --leak-check=full ./a.out
