all:
	gcc main.c lexer.c utils.c vector.c state.c vm.c postfix.c \
		emit.c ll1.c -O0 -g -o compiler -Wall
