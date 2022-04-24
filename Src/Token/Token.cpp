#include "Token.h"
#include <cassert>


Token::Token(const Token& token): tipo(token()),index(token.id()) {}

Token::Token(Tipo _tipo, long long int _index): tipo(_tipo),index(_index) {}
	
long long Token::id() const {
	assert(index>=0);	// Index inapropriado
	return index;
}
Tipo Token::operator()() const{	// Facilitar comparações
	return tipo;
}

void Token::operator=(Token& token) {
	this->tipo = token.tipo;
	this->index = token.index;
}
