all:
	gcc main.c lexer.c utils.c vector.c state.c vm.c postfix.c \
	emit.c ir.c parser.c tree.c ast.c -O0 -g -o compiler -Wall
