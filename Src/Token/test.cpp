#include "Token.h"
#include "../Sintatico/Sintatico.h"

#include <cassert>
#include <iostream>

using namespace std;

int main() {
	Token token(INT,1);
	assert(token()==INT);	// Testa operator()

	Token copy = token;	
	assert(copy() == token() && copy.id()==token.id());	// Testa operador =

	cout << "Teste da classe Token ok!" << endl;
	return 0;
}
