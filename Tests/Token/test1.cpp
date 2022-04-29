#include "../../Src/Token/Token.h"
#include "../../Src/Tipos.h"

#include <cassert>
#include <iostream>

#include "../test_format.cpp"

using namespace std;

bool test() {
	Token token(INT,1);
	assert(token()==INT);	// Testa operator()

	Token copy = token;	
	assert(copy() == token() && copy.id()==token.id());	// Testa operador =

	return true;
}


int main(int argn, char *argv[]) {
	Test::execute(argv[1],test);
	return 0;
}
