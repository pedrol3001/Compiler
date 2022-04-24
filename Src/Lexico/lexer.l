%{
#include <stdio.h>
#include <iostream>

#include "Lexico.h"


using namespace std;


Lexico* lex;	// Ponteiro aaaaaaaaaaaaaaaa
TabSim &ts = TabSim::getInstance();

Token match(string str, Tipo tipo) {
	Token token = ts.insert(tipo);	// Insere na tabela de símbolos
	lex->push_back(token);		// Insere no array de tokens
	ts[token.id()].insert((Atributo*)(new NameAtt(str)));	// Insere atributo de nome do tipo
	ts[token.id()].insert((Atributo*)(new StrAtt(yytext)));// Insere atributo de string lida
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

"+" { match("Soma", Tipo::SUM);}

"-" {match("Subtracao", Tipo::SUB);}

"*" {match("Multiplicacao", Tipo::MUL);}

"/" {match("Divisao", Tipo::DIV);}

"<" {match("Menor", Tipo::LESS);}

">" {match("Maior", Tipo::GREATER);}

"<=" {match("Menor ou igual", Tipo::LESSEQUAL);}

">=" {match("Maior ou igual", Tipo::GREATEREQUAL);}

"==" {match("Igualdade", Tipo::EQUAL);}

"!=" {match("Diferenca", Tipo::NOTEQUAL);}

"=" {match("Atribuicao", Tipo::ASSIGN);}

";" {match("Ponto e virgula", Tipo::SEMICOLON);}

"," {match("Virgula", Tipo::COMMA);}

"if" {match("if", Tipo::IF);}

"else" {match("else", Tipo::ELSE);}

"return" {match("return", Tipo::RETURN);}

"while" {match("while", Tipo::WHILE);}

"void" {match("void", Tipo::VOID);}

"int" {match("int", Tipo::INT);}

"float" {match("float", Tipo::FLOAT);}

"double" {match("double", Tipo::DOUBLE);}

"char" {match("char", Tipo::CHAR);}

"(" {match("(", Tipo::LPAREN);}

")" {match(")", Tipo::RPAREN);}

"[" {match("[", Tipo::LBRACKET);}

"]" {match("]", Tipo::RBRACKET);}

"{" {match("{", Tipo::LBRACE);}

"}" {match("}", Tipo::RBRACE);}

{Letra}({Digito}|{Letra}|_)* {match("Identificador", Tipo::ID);}

{Digito}+ {match("Inteiro", Tipo::C_INT);}

({Digito}+"."{Digito}*)|({Digito}*"."{Digito}+) {match("Ponto flutuante", Tipo::C_FLOAT);}

\"(\\\"|[^\"\n])*\" {match("String", Tipo::C_STRING);}

"/*"[^*]*"*"+([^/][^*]*"*"+)*"/" { match("Comentario", Tipo::COMMENT);}

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

