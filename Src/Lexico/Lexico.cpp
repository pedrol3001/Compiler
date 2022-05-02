#include "Lexico.h"

#include <cstdio>

#include "lex.yy.c"	// Include do código gerado pelo flex

Lexico::Lexico(std::string file): ok(true),count(0) {	
	// Analisa arquivo e constoi vetor de tokens
	// Usar flex
	yyin = fopen (file.c_str(),"r");
	if(yyin==NULL) {
		ok=false;
		return;
	}
	yyout = stdout; 
	lex = this;	// Argumento extra que inventei
	yylex();	
	fclose(yyin);	// Fecha o arquivo
	// OBS: não feche stdout
}

long long unsigned int Lexico::size() {
	return this->container.size();
}

void Lexico::reset() {
	count=0;
}

bool Lexico::operator>>(Token& token) {
	if(count < this->container.size()) {
		token = this->container[count++];
		return true;
	}
	return false;
}

bool Lexico::error() {return !ok;}

