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
	
	vector<Instrucao*> v;		
	
	// 1: int a;					// Pilha: a
	Token a_0 = ts.insert(ID);
	
	v.push_back(new Aloca(1));
	
	// 2: a = 2					// Pilha: a
	Token a_1 = ts.insert(ID);
		ts[a_1].insert(new VarEstatica(0,1));	//        ^
	Token c2_0 = ts.insert(C_INT);
		ts[c2_0].insert(new IntVal("2"));
	
	v.push_back(new Atribuicao(a_1,c2_0));
	
	// 3: int t1;					// Pilha: a t1
	Token t1_0 = ts.insert(ID);
	v.push_back(new Aloca(1));
	
	// 4: t1 = 2 + a				// Pilha: a t1
	Token t1_1 = ts.insert(ID);
		ts[t1_1].insert(new VarEstatica(0,1));	//           ^
	Token c2_1 = ts.insert(C_INT);
		ts[c2_1].insert(new IntVal("2"));
	Token a_2 = ts.insert(ID);
		ts[a_2].insert(new VarEstatica(1,1));	//        ^
	
	v.push_back(new Adicao(t1_1,c2_1,a_2));
	
	// 5: int t2;					// Pilha: a t1 t2
	Token t2_0 = ts.insert(ID);			
	v.push_back(new Aloca(1));
	
	// 6: t2 = t1*3				// Pilha: a t1 t2
	Token t2_1 = ts.insert(ID);
		ts[t2_1].insert(new VarEstatica(0,1));	//             ^
	Token t1_2 = ts.insert(ID);
		ts[t1_2].insert(new VarEstatica(1,1));	//          ^
	Token c3_0 = ts.insert(C_INT);	
		ts[c3_0].insert(new IntVal("3"));
	
	v.push_back(new Multiplicacao(t2_1,t1_2,c3_0));
	
	// 7: int t3;					// Pilha: a t1 t2 t3
	Token t3_0 = ts.insert(ID);
	v.push_back(new Aloca(1));
	
	// 8: t3 = t2-1				// Pilha: a t1 t2 t3
	Token t3_1 = ts.insert(ID);
		ts[t3_1].insert(new VarEstatica(0,1));	//                ^
	Token t2_2 = ts.insert(ID);
		ts[t2_2].insert(new VarEstatica(1,1));	//             ^
	Token c1_0 = ts.insert(C_INT);	
		ts[c1_0].insert(new IntVal("1"));
	
	v.push_back(new Subtracao(t3_1,t2_2,c1_0));
	
	// 9: a = t3/2		a=(((2+a)*3)-1)/2;	// Pilha: a t1 t2 t3
	Token a_3 = ts.insert(ID);
		ts[a_3].insert(new VarEstatica(3,1));	//        ^
	Token t3_2 = ts.insert(ID);
		ts[t3_2].insert(new VarEstatica(0,1));	//                ^
	Token c2_2 = ts.insert(C_INT);	
		ts[c2_2].insert(new IntVal("2"));
	
	v.push_back(new Divisao(a_3,t3_2,c2_2));
	
	// 10: ~int 3					// Pilha: a 
	v.push_back(new Desaloca(3));
	
	
	// 11: goto 12: 				// Pilha: a 
	v.push_back(new Salto(11));
	
	// 12:if a < 100 goto 2:			// Pilha: a 
	Token a_4 = ts.insert(ID);	
		ts[a_4].insert(new VarEstatica(0,1));	//        ^
	Token c100_0 = ts.insert(C_INT);	
		ts[c100_0].insert(new IntVal("100"));
	
	v.push_back(new Blt(a_4,c100_0,2));
	
	// Teste:	
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
