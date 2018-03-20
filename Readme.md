This is a simple compiler I've made for study purpose.

Arguments:
	--file file // Program to execute
	--parsetree file // Parse tree
	--tokens file // Dump tokens to file

Grammar:

```c
STATEMENT -> ASSIGNMENT STATEMENT | IFBLOCK STATEMENT | WHILELOOP STATEMENT
	| ASSIGNMENT | IFBLOCK | WHILELOOP
ASSIGNMENT -> ID = EXPR
EXPR = ID | NUM | ID + EXPR| ID - EXPR| ID * EXPR | ID / EXPR
	| NUM + EXPR | NUM - EXPR | NUM * EXPR | NUM / EXPR
IFBLOCK -> if (EXPR) then STATEMENT end
	| if (EXPR) then STATEMENT else STATEMENT end
WHILELOOP -> while (EXPR == EXPR) do STATEMENT end
ID -> [a-Z] ID | [0-9] ID | [0-9] | [a-Z]
NUM -> [0-9] NUM | [0-9]
```

Possible program:
```c
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
```

Parse tree:
![Parse tree:](https://i.imgur.com/WSoesKC.jpg)

TODO:
update grammar
rewrite lexer
rewrite parse tree
write vm: +-
ast: -
tac: -
ssa: -
tac to bc: -
