all:
	gcc main.c front/lex.c utils.c misc/vector.c state.c vm.c postfix.c \
	emit.c ir.c parser.c tree.c ast.c bytecode.c \
	-O0 -g -o compiler -Wall
