#ifndef Lexico_h
#define Lexico_h

#include "../Token/Token.h"
#include "../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <vector>
#include <string>

struct Lexico: public std::vector<Token> {
	Lexico(std::string file);	// Analisa arquivo e constoi vetor de tokens
	bool error();
	void reset();
	bool operator>>(Token& token);
	private:
		long long unsigned int count;
		bool ok;
};

struct StrAtt: public Atributo {
	std::string str;
	StrAtt(std::string _str);
};



struct NameAtt: public Atributo {
	std::string str;
	NameAtt(std::string _str);
};

#endif
