#include "Token.h"
#include <cassert>

Token::Token(Tipo tipo, long long int index): tipo(tipo),index(index) {}
	
long long Token::id() const {
	assert(index>=0);	// Index inapropriado
	return index;
}
Tipo Token::operator()() {	// Facilitar comparações
	return tipo;
}

void Token::operator=(Token& token) {
	this->tipo = token.tipo;
	this->index = token.index;
}
