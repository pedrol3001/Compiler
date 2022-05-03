#include "../../Src/Sintatico/Sintatico.h"
#include <iostream>

#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {
	FILE* f = fopen("Resources/code6.txt","r");
	Lexico lexico(f,stdout,false);
	fclose(f);
	if(lexico.error()) {
		tester.error() << "Nao foi possivel abrir o arquivo!" << endl;
		return false;
	}
	Sintatico sintatico;
	sintatico.analisar(lexico);
	
	tester.comment() << "Status retornado pelo bison" << endl;
	if(sintatico.good())	
		tester.normal() << "Aceito!" << endl;
	else
		tester.normal() << "Rejeitado!" << endl;
	return true;
}

int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
