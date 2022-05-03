#ifndef Lexico_h
#define Lexico_h

#include "../Token/Token.h"
#include "../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <vector>
#include <string>
#include <cstdio>

struct Lexico {
	Lexico(FILE* ifile, FILE* ofile, bool _verbose=false);	// Analisa arquivo e constoi vetor de tokens
	std::vector<Token> container;
	bool error();
	bool good();
	void reset();
	bool operator>>(Token& token);
	long long unsigned int size();
	private:
		long long unsigned int count;
		bool ok;
};

#endif
