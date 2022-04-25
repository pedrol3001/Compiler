#include "TabelaDeSimbolos.h"

#include "../Token/Token.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

class meuAtt1: public Atributo {
	public: 	
		meuAtt1(): Atributo("meuAtt1")	{}
		void print1() {cout << "Hello world!" << endl;}
};

class meuAtt2: public Atributo {
	public: 
		meuAtt2(): Atributo("meuAtt2") {}
		void print2() {cout << "Ola mundo!" << endl;}
};

int main() {
	TabSim& ts = TabSim::getInstance();
	
	Token t0 = ts.insert(INT);
	Token t1 = ts.insert(FLOAT);
	Token t2 = ts.insert(FLOAT);
	
	// Verificar funcionamento dos indices
	assert(t0.id()==0);
	assert(t1.id()==1);
	assert(t2.id()==2);
	
	ts[t0.id()].insert((Atributo*)new meuAtt1);
	ts[t0.id()].insert((Atributo*)new meuAtt2);
	
	meuAtt1& ma1 = *((meuAtt1*)(ts[t0.id()]["meuAtt1"]));
	assert(ma1.nome == "meuAtt1"); 	// Verificar acesso direto
	
	Simbolo& s = ts[t0.id()];
	meuAtt2& ma2 = *((meuAtt2*)(s["meuAtt2"]));
	assert(ma2.nome == "meuAtt2");
	
	ma1.print1(); ma2.print2();
	
	// Verificar memory leaks (valgrind?)!
	
	cout << "Teste da classe TabelaDeSimbolos ok! (Lembre-se de testar memory leaks)" << endl;
	return 0;
}
