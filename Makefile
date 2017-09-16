all:
	gcc main.c lexer.c utils.c vector.c state.c vm.c postfix.c \
	emit.c parser.c tree.c -O0 -g -o compiler -Wall
