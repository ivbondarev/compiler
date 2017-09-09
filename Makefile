all:
	gcc main.c lexer.c utils.c vector.c state.c -O0 -g -o compiler
