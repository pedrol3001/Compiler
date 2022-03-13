all: lex.yy.c parser.tab.c
	gcc -o $@ parser.tab.c lex.yy.c -lfl

lex.yy.c: ./lexical/lexer.l
	flex ./lexical/lexer.l

parser.tab.c: ./sintatical/parser.y
	bison -d ./sintatical/parser.y

test: all
	./all ./tests/test1.txt