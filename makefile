main: 
	gcc -Wall -ggdb3 -g -std=c99 main.c op_codes_initializer.c

cppcheck:
	cppcheck --enable=warning --suppress=missingIncludeSystem .

valgrind:
	valgrind --leak-check=full --leak-resolution=high --track-origins=yes --show-leak-kinds=all ./a.out
