%{
#include <stdio.h>
#include <iostream>

#include "Lexico.h"
#include "../Token/Token.h"
#include "../Tipos.h"


using namespace std;

bool verbose=false;
Lexico* lex;	// Ponteiro aaaaaaaaaaaaaaaa
TabSim &ts = TabSim::getInstance();

string getToken() {
	string token(yytext);
	token = token.substr(0,yyleng);
	return token;
}

Token match(string str, int tipo) {
	if(verbose)
		fprintf(yyout," [%s] ",yytext); // Util para debug
	Token token = ts.insert(tipo);	// Insere na tabela de símbolos
	lex->container.push_back(token);		// Insere no array de tokens
	ts[token].insert((Atributo*)(new NameAtt(str)));	// Insere atributo de nome do tipo
	ts[token].insert((Atributo*)(new StrAtt(getToken())));// Insere atributo de string lida
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

{Letra}({Digito}|{Letra}|_)* {
	Token token = match("Identificador", ID);
	ts[token].insert((Atributo*)(new IdVal(getToken())));	
}

{Digito}+ {
	Token token = match("Inteiro", C_INT);
	ts[token].insert((Atributo*)(new IntVal(getToken())));
}

({Digito}+"."{Digito}*)|({Digito}*"."{Digito}+) {
	Token token = match("Ponto flutuante", C_FLOAT);
	ts[token].insert((Atributo*)(new DoubleVal(getToken())));
}

\"(\\\"|[^\"\n])*\" {
	Token token = match("String", C_STRING);
	ts[token].insert((Atributo*)(new StringVal(getToken())));
}

"/*"[^*]*"*"+([^/][^*]*"*"+)*"/" { match("Comentario", COMMENT);}

{Space}* {}

. {
	return -1;
	BEGIN(INVALIDO_UMA_LINHA); 
	yyless(yyleng-1);
}
"/*" {
	BEGIN(INVALIDO_VARIAS_LINHAS);
	yyless(yyleng-2);
}

<<EOF>> {return 0;}


<INVALIDO_VARIAS_LINHAS>(.|\n)+ {
	//match("Invalido",NOT_INITIALIZED);
	BEGIN(INITIAL);
}


<INVALIDO_UMA_LINHA>.+ {
	//match("Invalido",NOT_INITIALIZED);
	BEGIN(INITIAL);
}

%%

/*

*/

