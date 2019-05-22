main: 
	gcc -Wall -ggdb3 -g -std=c99 main.c op_codes_initializer.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

valgrind:
	valgrind --leak-check=full --leak-resolution=high --track-origins=yes --show-leak-kinds=all ./a.out
