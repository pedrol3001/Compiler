#include "../../Src/Token/Token.h"
#include "../../Src/Tipos.h"

#include <cassert>
#include <iostream>

#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {
	Token token(INT,1);
	if(!(token()==INT)) {
		tester.error() << "Erro no operador Token::operator()\n";
		return false;
	}

	Token copy = token;	
	if(!(copy() == token() && copy.id()==token.id())) { // Testa operador =
		tester.error() << "Erro no operador Token::operator=\n";
		return false;	
	}

	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
