%{
#include "parser.tab.h"
#include <stdio.h>
%}
/*Definitions*/
Digito [0-9]
Letra [a-zA-Z]
Sinal [-+]
Space [\n\t ]
NotSpace [^\n\t ]

%x INVALIDO_VARIAS_LINHAS
%x INVALIDO_UMA_LINHA

/*Rules*/
%{
	void match(char* tipo) {
		fprintf(yyout,"[%s: %s]\n",tipo,yytext);
	}
%}
%%

"+" {
	match("Soma");
	return SUM;
}

"-" {
	match("Subtracao");
	return SUB;
}

"*" {
	match("Multiplicacao");
	return MUL;
}

"/" {
	match("Divisao");
	return DIV;
}

"<" {
	match("Menor");
	return LESS;
}

">" {
	match("Maior");
	return GREATER;
}

"<=" {
	match("Menor ou igual");
	return LESSEQUAL;
}

">=" {
	match("Maior ou igual");
	return GREATEREQUAL;
}

"==" {
	match("Igualdade");
	return EQUAL;
}

"!=" {
	match("Diferenca");
	return NOTEQUAL;
}

"=" {
	match("Atribuicao");
	return ASSIGN;
}

";" {
	match("Ponto e virgula");
	return SEMICOLON;
}

"," {
	match("Virgula");
	return COMMA;
}

"if" {
	match("if");
	return IF;
}

"else" {
	match("else");
	return ELSE;
}

"return" {
	match("return");
	return RETURN;
}

"while" {
	match("while");
	return WHILE;
}

"void" {
	match("void");
	return VOID;
}

"int" {
	match("int");
	return INT;
}

"float" {
	match("float");
}

"double" {
	match("double");
}

"char" {
	match("char");
}

"(" {
	match("(");
	return LPAREN;
}

"print" {
	match("print");
	return OUTPUT;
}

"input" {
	match("input");
	return INPUT;
}

")" {
	match(")");
	return RPAREN;
}

"[" {
	match("[");
	return LBRACKET;
}

"]" {
	match("]");
	return RBRACKET;
}

"{" {
	match("{");
	return LBRACE;
}

"}" {
	match("}");
	return RBRACE;
}

{Letra}({Digito}|{Letra}|_)* {
	match("Identificador");
	return ID;
}

{Digito}+ {
	match("Inteiro");
	return INTEGER;
}

({Digito}+"."{Digito}*)|({Digito}*"."{Digito}+) {
	match("Ponto flutuante");
	return FLOAT;
}

\"(\\\"|[^\"\n])*\" {
	match("String");
	return STRING;
}

"/*"[^*]*"*"+([^/][^*]*"*"+)*"/" {
	match("Comentario");
}

{Space}* {

}

"/*" {
	BEGIN(INVALIDO_VARIAS_LINHAS);
	yyless(yyleng-2);
}
<INVALIDO_VARIAS_LINHAS>(.|\n)+ {
	match("Invalido");
	BEGIN(INITIAL);
}

. {
	BEGIN(INVALIDO_UMA_LINHA); yyless(yyleng-1);
}

<INVALIDO_UMA_LINHA>.+ {
	match("Invalido");
	BEGIN(INITIAL);
}

%%
//
//int yywrap ();
//
///*User*/
//int main(argc, argv)
//    int argc;
//    char **argv;
//        {
//        ++argv; --argc; 	    /* abre arquivo de entrada se houver */
//        if(argc > 0)
//            yyin = fopen(argv[0],"rt");
//        else
//            yyin = stdin;    /* cria arquivo de saida se especificado */
//        if(argc > 1)
//            yyout = fopen(argv[1],"wt");
//        else
//            yyout = stdout;
//        yylex(); /* scanner gerado por Flex */
//        fclose(yyin);
//        fclose(yyout);
//        }
//
//int yywrap() {
//	return 1;
//}