#include "Token.h"
#include "../Tipos.h"
#include <cassert>

Token::Token(): tipo(NOT_INITIALIZED),index(-1) {}

Token::Token(int _tipo, long long int _index): tipo(_tipo),index(_index) {}
	
long long Token::id() const {
	assert(index>=0);	// Index inapropriado ou nao inicializado
	return index;
}
int Token::operator()() const{	// Facilitar comparações
	return tipo;
}
