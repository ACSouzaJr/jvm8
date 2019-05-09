main: 
	gcc -Wall -g main.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .
