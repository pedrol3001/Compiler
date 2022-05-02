#include "../../Src/Gerador/Gerador.h"

#include "../../Src/TabelaDeSimbolos/TabelaDeSimbolos.h"
#include "../../Src/Tipos/Tipos.h"

#include <iostream>
#include <sstream>
#include "../test_format.cpp"

using namespace std;
using namespace Addr3;

bool test(Test& tester) {
	stringstream output;

	TabSim& ts = TabSim::getInstance();
	
	Token a = ts.insert(ID);
	Token b = ts.insert(ID);
	Token t1 = ts.insert(ID);
	Token t2 = ts.insert(ID);
	Token c1 = ts.insert(C_INT);
	Token c2 = ts.insert(C_INT);
	Token c3 = ts.insert(C_INT);

	vector<Instrucao*> v;
	// Declaracao
	v.insert(new Declaracao());	// 0: int a,b;		-	int a,b;
	// Operacoes
	v.insert(new Assignment());	// 1: a = 2		-	a=2;
	v.insert(new Adicao());	// 2: t1 = 2 + a	-
	v.insert(new Multiplicacao());	// 3: t2 = t1*3	|	b = ((2+a)*3)-1;
	v.insert(new Subtracao());	// 4: b = t2-1		-
	v.insert(new Divisao());	// 5: a = b/2		-	a=b/2;
	// Salto incondicional
	v.insert(new Salto(7));	// 6: goto 7: 
	// Saltos Condicionais	
	v.insert(new Beq(1));		// 7: if a+b < 100 goto 1:
	
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
