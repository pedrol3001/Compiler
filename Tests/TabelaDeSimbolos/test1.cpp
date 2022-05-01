#include "../../Src/TabelaDeSimbolos/TabelaDeSimbolos.h"

#include "../../Src/Token/Token.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>

#include "../test_format.cpp"

using namespace std;

class meuAtt1: public Atributo {
	Test& tester;
	public: 	
		meuAtt1(Test& _tester): Atributo("meuAtt1"), tester(_tester) {}
		void print1() {
			tester.normal() << "Hello world!\n" << endl;
		}
};

class meuAtt2: public Atributo {
	Test& tester;
	public: 
		meuAtt2(Test& _tester): Atributo("meuAtt2"), tester(_tester) {}
		void print2() {
			tester.normal() << "Ola mundo!\n" << endl;
		}
};

bool test(Test& tester) {
	TabSim& ts = TabSim::getInstance();
	
	Token t0 = ts.insert(INT);
	Token t1 = ts.insert(FLOAT);
	Token t2 = ts.insert(FLOAT);
	
	// Verificar funcionamento dos indices
	if(!(t0.id()==0 && t1.id()==1 && t2.id()==2)) {
		tester.error() << "Falha na geracao de indices dos tokens" << endl;
		return false;
	}
	
	ts[t0].insert((Atributo*)new meuAtt1(tester));
	ts[t0].insert((Atributo*)new meuAtt2(tester));
	
	meuAtt1& ma1 = *((meuAtt1*)(ts[t0]["meuAtt1"]));
	if(!(ma1.nome == "meuAtt1")) {		
		tester.error() << "Falha na insercao de meuAtt1" << endl;
		return false;	
	}
	
	Simbolo& s = ts[t0];
	meuAtt2& ma2 = *((meuAtt2*)(s["meuAtt2"]));
	
	if(!(ma2.nome == "meuAtt2")) {		
		tester.error() << "Falha na insercao de meuAtt2" << endl;
		return false;	
	}
	
	tester.comment() << "Metodos de meuAtt1 e meuAtt2, respectivamente." << endl;
	ma1.print1(); ma2.print2();
	
	tester.comment() << "Lembre-se de testar memory leaks! (debug=yes?)" << endl;
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
