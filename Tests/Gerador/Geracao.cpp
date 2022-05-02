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
	Token t1 = ts.insert(ID);
	ts[t1].insert(new VarEstatica(1));
	Token t2 = ts.insert(ID);
	ts[t2].insert(new VarEstatica(1));
	
	Token c1 = ts.insert(C_INT);
	ts[c1].insert(new IntVal("1"));
	
	Token c2 = ts.insert(C_INT);
	ts[c2].insert(new IntVal("2"));
	
	Token c3 = ts.insert(C_INT);
	ts[c3].insert(new IntVal("3"));

	vector<Instrucao*> v;
	v.push_back(new Empilha(a));			// 1: int a;		-	int a,b;
	v.push_back(new Empilha(b));			// 2: int b;		-	
	v.push_back(new Assignment(a,c2));		// 3: a = 2		-	a=2;
	v.push_back(new Empilha(t1));			// 4: int t1;		-	
	v.push_back(new Adicao(t1,c2,a));		// 5: t1 = 2 + a	-
	v.push_back(new Empilha(t2));			// 6: int t2;		-	
	v.push_back(new Multiplicacao(t2,t1,c3));	// 7: t2 = t1*3	|	b = ((2+a)*3)-1;
	v.push_back(new Subtracao(b,c2,c1));		// 8: b = t2-1		-
	v.push_back(new Divisao(a,b,c2));		// 9: a = b/2		-	a=b/2;
	v.push_back(new Salto(8));			// 10: goto 11: 
	v.push_back(new Beq(2));			// 11:if a+b = 100 goto 3:
	
	stringstream output;
	
	for(Instrucao* instrucao: v) {
		list<Assembly*> code = instrucao->gera_codigo();
		
		for(Assembly* assembly: code)
			output << assembly->str() << '\n';
		
		for(Assembly* assembly: code)
			delete assembly;
	}
	output << "HALT 0,0,0" << endl;

	tester.comment() << "Output: ..............." << endl;
	tester.normal() << output.str();
	tester.comment() << "Fim do output ........." << endl;
	
	tester.log() << "Partes deste teste nao sao verificas automaticamente" << endl;
	
	for(Instrucao* instrucao: v) 
		delete instrucao;
	
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
