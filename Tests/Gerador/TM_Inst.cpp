#include "../../Src/Gerador/Gerador.h"
#include "../../Src/Gerador/Gerador.h"

#include <iostream>
#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {
	TM::ADD add(TM::r0, TM::r1, TM::r2);
	tester.comment() << "Testando instrucao ADD (tipo RO)" << endl;
	tester.normal() << add.str() << endl;
	if(add.str()!="ADD 0,1,2")
		tester.log() << "Algo de errado? Esperado->|ADD 0,1,2|" << endl;
	
	TM::JEQ jeq(TM::r0, -20, TM::r2);
	tester.comment() << "Testando instrucao JEQ (tipo RR)" << endl;
	tester.normal() << jeq.str() << endl;
	if(jeq.str()!="JEQ 0,-20(2)")
		tester.log() << "Algo de errado? Esperado->|JEQ 0,-20(2)|" << endl;
	
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
