#ifndef Token_h
#define Token_h

#include "../TabelaDeSimbolos/TabelaDeSimbolos.h"

struct Token {	
	Token(Tipo tipo, long long int index);
	
	long long id() const;	// Obter índice, mas pode ser desnecessário de qualquer forma.
	
	Tipo operator()();		// Facilitar comparações
	void operator=(Token& token);	// Apenas copia tudo
	
	private:
		Tipo tipo;		
		long long int index;
};	

#endif
