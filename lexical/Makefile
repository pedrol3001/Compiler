lex.yy.c: lexical.l
	flex lexical.l
	
lex_test: lex.yy.c
	gcc lex.yy.c -o lex_test

TESTE = teste.txt
test: lex_test $(TESTE)
	./lex_test $(TESTE)
	
