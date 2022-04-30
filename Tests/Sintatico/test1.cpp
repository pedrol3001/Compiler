#include "../../Src/Sintatico/Sintatico.h"
#include <iostream>

#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {
	Lexico lexico("Resources/code6.txt");
	if(lexico.error()) {
		tester.error() << "Nao foi possivel abrir o arquivo!" << endl;
		return false;
	}
	Sintatico sintatico(lexico);
	
	tester.comment() << "Status retornado pelo bison" << endl;
	tester.normal() << "Status: " << sintatico.status << endl;
	return true;
}

int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
