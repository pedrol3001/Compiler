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
	v.push_back(new Declaracao(a));		// 0: int a;		-	int a,b;
	v.push_back(new Declaracao(b));		// 1: int b;		-	
	// Operacoes
	v.push_back(new Assignment(a,c2));		// 2: a = 2		-	a=2;
	v.push_back(new Adicao(t1,c2,a));		// 3: t1 = 2 + a	-
	v.push_back(new Multiplicacao(t2,t1,c3));	// 4: t2 = t1*3	|	b = ((2+a)*3)-1;
	v.push_back(new Subtracao(b,c2,c1));		// 5: b = t2-1		-
	v.push_back(new Divisao(a,b,c2));		// 6: a = b/2		-	a=b/2;
	// Salto incondicional
	v.push_back(new Salto(8));			// 7: goto 8: 
	// Saltos Condicionais	
	v.push_back(new Beq(2));			// 8: if a+b < 100 goto 2:
	
	for(Instrucao* instrucao: v) {
		list<Assembly*> code = instrucao->gera_codigo();
		
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
