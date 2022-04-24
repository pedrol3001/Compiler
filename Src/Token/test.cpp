#include "Token.h"
#include "../Tipos.h"

#include <cassert>
#include <iostream>

using namespace std;

int main() {
	Token token(Tipo::INT,1);
	assert(token()==Tipo::INT);	// Testa operator()

	Token copy = token;	
	assert(copy() == token() && copy.id()==token.id());	// Testa operador =

	cout << "Teste da classe Token ok!" << endl;
	return 0;
}
