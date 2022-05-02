#include "../../Src/Gerador/Gerador.h"

#include <iostream>
#include <sstream>
#include "../test_format.cpp"

using namespace std;
using namespace Addr3;

bool test(Test& tester) {
	stringstream output;

	vector<Instrucao*> v;
	
	for(Instrucao* instrucao: v) {
		std::vector<Assembly*> code = instrucao->gera_codigo();
		
		for(Assembly* assembly: code)
			output << assembly->str() << '\n';
		
		for(Assembly* assembly: code)
			delete assembly;
	}

	tester.comment() << "Output: ..............." << endl;
	tester.normal() << output.str();
	tester.comment() << "Fim do output ........." << endl;
	
	tester.log() << "Esse teste nao eh verificado automaticamente" << endl;
	
	for(Instrucao* instrucao: v) 
		delete instrucao;
	
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
