#include "Lexico.h"

#include <cstdio>
#include <iostream>

#include "lex.yy.c"	// Include do código gerado pelo flex

Lexico::Lexico(FILE* ifile, FILE* ofile): ok(true),count(0) {	
	// Analisa arquivo e constoi vetor de tokens
	// Usar flex
	yyin = ifile;
	yyout = ofile; 
	lex = this;	// Argumento extra que inventei
	if(yylex()) {
		cout << "Erro lexico!" << endl;
		ok=false;
		return;	
	}	
	ok=true;
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

bool Lexico::good() {return ok;}

bool Lexico::error() {return !ok;}

