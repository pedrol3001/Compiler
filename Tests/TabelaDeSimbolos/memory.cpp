#include "../../Src/Gerador/Gerador.h"

#include "../../Src/TabelaDeSimbolos/TabelaDeSimbolos.h"
#include "../../Src/Tipos.h"

#include <iostream>
#include <sstream>
#include <list>
#include "../test_format.cpp"

using namespace std;
using namespace Addr3;

bool test(Test& tester) {
	TabSim& ts = TabSim::getInstance();
	
	Token a = ts.insert(ID);
	ts[a].insert(new VarEstatica(1));
	Token b = ts.insert(ID);
	ts[b].insert(new VarEstatica(1));
	
	tester.comment() << "Testando contador sp_counter" << endl;
	tester.normal() << VarEstatica::sp_counter << endl;
	if(VarEstatica::sp_counter!=2) {
		tester.error() << "VarEstatica::sp_counter incorreto!" << endl;
		tester.log() << "VarEstatica::sp_counter deveria ser 2!" << endl;
		return false;
	}
	
	Token t1 = ts.insert(ID);
	ts[t1].insert(new VarEstatica(1));
	
	tester.comment() << "Testando Variavel::getOffset() sp_counter" << endl;
	
	tester.normal() << ((VarEstatica*) ts[t1]["VarEstatica"])->getOffset() << endl;
	if(((VarEstatica*) ts[t1]["VarEstatica"])->getOffset() != 0) {
		tester.error() << "Variavel::getOffset() incorreto!" << endl;
		tester.log() << "Variavel::getOffset() deveria retornar 0!" << endl;
		return false;
	}
	
	Token t2 = ts.insert(ID);
	ts[t2].insert(new VarEstatica(1));
	
	tester.normal() << ((VarEstatica*) ts[t1]["VarEstatica"])->getOffset() << endl;
	if(((VarEstatica*) ts[t1]["VarEstatica"])->getOffset() != 1) {
		tester.error() << "Variavel::getOffset() incorreto!" << endl;
		tester.log() << "Variavel::getOffset() deveria retornar 1!" << endl;
		return false;
	}
	
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
