main: 
	gcc -Wall -g -std=c99 main.c op_codes_initializer.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./a.out
