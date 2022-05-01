%{
#include <stdio.h>
#include <iostream>

#include "Lexico.h"
#include "../Tipos.h"


using namespace std;


Lexico* lex;	// Ponteiro aaaaaaaaaaaaaaaa
TabSim &ts = TabSim::getInstance();

Token match(string str, int tipo) {
	Token token = ts.insert(tipo);	// Insere na tabela de símbolos
	lex->container.push_back(token);		// Insere no array de tokens
	ts[token].insert((Atributo*)(new NameAtt(str)));	// Insere atributo de nome do tipo
	ts[token].insert((Atributo*)(new StrAtt(yytext)));// Insere atributo de string lida
	return token;
}

%}

%option noyywrap

/*Definitions*/
Digito [0-9]
Letra [a-zA-Z]
Sinal [-+]
Space [\n\t ]
NotSpace [^\n\t ]

%x INVALIDO_VARIAS_LINHAS
%x INVALIDO_UMA_LINHA

/*Rules*/

%%

"+" { match("Soma", SUM);}

"-" {match("Subtracao", SUB);}

"*" {match("Multiplicacao", MUL);}

"/" {match("Divisao", DIV);}

"<" {match("Menor", LESS);}

">" {match("Maior", GREATER);}

"<=" {match("Menor ou igual", LESSEQUAL);}

">=" {match("Maior ou igual", GREATEREQUAL);}

"==" {match("Igualdade", EQUAL);}

"!=" {match("Diferenca", NOTEQUAL);}

"=" {match("Atribuicao", ASSIGN);}

";" {match("Ponto e virgula", SEMICOLON);}

"," {match("Virgula", COMMA);}

"if" {match("if", IF);}

"else" {match("else", ELSE);}

"return" {match("return", RETURN);}

"while" {match("while", WHILE);}

"void" {match("void", VOID);}

"int" {match("int", INT);}

"float" {match("float", FLOAT);}

"double" {match("double", DOUBLE);}

"char" {match("char", CHAR);}

"(" {match("(", LPAREN);}

")" {match(")", RPAREN);}

"[" {match("[", LBRACKET);}

"]" {match("]", RBRACKET);}

"{" {match("{", LBRACE);}

"}" {match("}", RBRACE);}

{Letra}({Digito}|{Letra}|_)* {match("Identificador", ID);}

{Digito}+ {match("Inteiro", C_INT);}

({Digito}+"."{Digito}*)|({Digito}*"."{Digito}+) {match("Ponto flutuante", C_FLOAT);}

\"(\\\"|[^\"\n])*\" {match("String", C_STRING);}

"/*"[^*]*"*"+([^/][^*]*"*"+)*"/" { match("Comentario", COMMENT);}

{Space}* {}

"/*" {
	BEGIN(INVALIDO_VARIAS_LINHAS);
	yyless(yyleng-2);
}
<INVALIDO_VARIAS_LINHAS>(.|\n)+ {
	//match("Invalido");
	BEGIN(INITIAL);
}

. {
	BEGIN(INVALIDO_UMA_LINHA); yyless(yyleng-1);
}

<INVALIDO_UMA_LINHA>.+ {
	//match("Invalido");
	BEGIN(INITIAL);
}

%%

