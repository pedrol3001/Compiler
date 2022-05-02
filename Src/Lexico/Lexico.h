#ifndef Lexico_h
#define Lexico_h

#include "../Token/Token.h"
#include "../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <vector>
#include <string>

struct Lexico {
	Lexico(std::string file);	// Analisa arquivo e constoi vetor de tokens
	std::vector<Token> container;
	bool error();
	void reset();
	bool operator>>(Token& token);
	long long unsigned int size();
	private:
		long long unsigned int count;
		bool ok;
};

#endif
