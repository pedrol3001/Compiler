lex.yy.c: lexical.l
	flex lexical.l

TESTE = teste.txt
test: lex.yy.c $(TESTE)
	gcc lex.yy.c -o lex_test
	./lex_test $(TESTE)
	
build: lex.yy.c
	gcc lex.yy.c -o lex
	
all: build
	rm lex.yy.c
	
