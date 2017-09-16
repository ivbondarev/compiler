This is a simple compiler I've made for study purpose.

Implementation based on recursive descent parser with this LL(3) grammar:

PROG -> STATEMENT
STATEMENT -> ASSIGN STATEMENT | IF STATEMENT | WHILE STATEMENT |
	ASSIGN EOS | IF EOS| WHILE EOS
ASSIGN -> ID = NUM | ID = ID
IF -> if ( COND ) then STATEMENT else STATEMENT end
WHILE -> while ( COND ) do STATEMENT end
COND -> ID == ID | ID == NUM
ID -> name1
NUM -> num1

Possible program:

var1 = 3
var2 = 8
var3 = var2

if (var3 == 8) then
	var1 = 1
else
	var1 = 2

	if (var == 1) then
		var2 = 1
		var3 = 1
	else
		var3 = var2
		var2 = 352
	end
end

![Parse tree:](https://i.imgur.com/WSoesKC.jpg)

TODO:
lexer+
parse tree+
vm+-
ast-
tac-
ssa-
