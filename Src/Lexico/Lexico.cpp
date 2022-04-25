#include "Lexico.h"

#include <cstdio>

#include "lex.yy.c"	// Include do código gerado pelo flex

Lexico::Lexico(std::string file): ok(true) {	
	// Analisa arquivo e constoi vetor de tokens
	// Usar flex
	yyin = fopen (file.c_str(),"r");
	yyout = stdout; 
	lex = this;	// Argumento extra que inventei
	yylex();	
	fclose(yyin);	// Fecha o arquivo
	// OBS: não feche stdout
}

bool Lexico::error() {return !ok;}


StrAtt::StrAtt(string _str): Atributo("StrAtt"), str(_str) {}

NameAtt::NameAtt(string _str): Atributo("NameAtt"), str(_str) {}
