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

long long int linha=0,coluna=0,posicao=0,erros_lexico=0;

string getToken() {
	string token(yytext);
	token = token.substr(0,yyleng);
	return token;
}

void update() {
	coluna += yyleng;
	posicao += yyleng;
}

Token match(string str, int tipo) {
	if(verbose)
		fprintf(yyout," [%lld(l%lld,c%lld): %s] ",posicao,linha,coluna,yytext); // Util para debug
	Token token = ts.insert(tipo);	// Insere na tabela de símbolos
	lex->container.push_back(token);		// Insere no array de tokens
	ts[token].insert((Atributo*)(new StrAtt(getToken(),linha,coluna)));// Insere atributo de string lida
	return token;
}

%}

%option noyywrap

/*Definitions*/
Digito [0-9]
Letra [a-zA-Z]
Sinal [-+]
Space [\t \r]

%x INVALIDO_VARIAS_LINHAS
%x INVALIDO_UMA_LINHA

/*Rules*/

%%

"+" {match("Soma", SUM); update(); }

"-" {match("Subtracao", SUB); update(); }

"*" {match("Multiplicacao", MUL); update(); }

"/" {match("Divisao", DIV); update(); }

"<" {match("Menor", LESS); update(); }

">" {match("Maior", GREATER); update(); }

"<=" { match("Menor ou igual", LESSEQUAL); update(); }

">=" { match("Maior ou igual", GREATEREQUAL); update(); }

"==" { match("Igualdade", EQUAL); update(); }

"!=" { match("Diferenca", NOTEQUAL); update(); }

"=" { match("Atribuicao", ASSIGN); update(); }

";" { match("Ponto e virgula", SEMICOLON); update(); }

"," { match("Virgula", COMMA); update(); }

"if" { match("if", IF); update(); }

"else" { match("else", ELSE); update(); }

"return" { match("return", RETURN); update(); }

"while" { match("while", WHILE); update(); }

"void" { match("void", VOID); update(); }

"int" { match("int", INT); update(); }

"float" { match("float", FLOAT); update(); }

"double" { match("double", DOUBLE); update(); }

"char" { match("char", CHAR); update(); }

"input" { match("input", INPUT); update(); }

"output" { match("output", OUTPUT); update(); }

"(" { match("(", LPAREN); update(); }

")" { match(")", RPAREN); update(); }

"[" {match("[", LBRACKET); update(); }

"]" {match("]", RBRACKET); update(); }

"{" {match("{", LBRACE); update(); }

"}" {match("}", RBRACE); update(); }

{Letra}({Digito}|{Letra}|_)* {
	Token token = match("Identificador", ID);
	ts[token].insert((Atributo*)(new IdVal(getToken())));	
	update(); 
}

{Digito}+ {
	Token token = match("Inteiro", C_INT);
	ts[token].insert((Atributo*)(new IntVal(getToken())));
	update(); 
}

({Digito}+"."{Digito}*)|({Digito}*"."{Digito}+) {	
	Token token = match("Ponto flutuante", C_FLOAT);
	ts[token].insert((Atributo*)(new DoubleVal(getToken())));
	update(); 
}

\"(\\\"|[^\"\n])*\" {
	Token token = match("String", C_STRING);
	ts[token].insert((Atributo*)(new StringVal(getToken())));
	update(); 
}

"/*"[^*]*"*"+([^/][^*]*"*"+)*"/" { if(verbose) fprintf(yyout," [%lld(l%lld,c%lld): %s] ",posicao,linha,coluna,yytext);  update(); } // Util para debug}

{Space}* {update(); }

\n {linha++; coluna=0; if(verbose) fprintf(yyout,"\n");}

. { 
	fprintf(yyout,"Erro léxico na linha %lld, coluna %lld: %s\n",linha+1,coluna+1,yytext);
	update();
	erros_lexico++;
	}	// Erro lexico

<<EOF>> {
	if(erros_lexico){
		fprintf(yyout,"%lld erros léxico encontrados.\n", erros_lexico);
		return -1;
	}
	return 0;
	}

%%

/*

*/

