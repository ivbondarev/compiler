all:
	gcc main.c front/lex.c utils.c containers/vector.c state.c vm.c postfix.c \
	emit.c ir.c parser.c containers/tree.c ast.c bytecode.c \
	-O0 -g -std=c99 -Wall -o compiler
