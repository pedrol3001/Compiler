#include "TabelaDeSimbolos.h"
#include <cassert>

using namespace std;


/* A tabela de símbolos é um vetor de conjuntos de atributos, com nome em forma de string*/
TabelaDeSimbolos& TabelaDeSimbolos::getInstance() {	// Caso seja usada como Singleton
	static TabelaDeSimbolos instance;
	return instance;
}
TabelaDeSimbolos::TabelaDeSimbolos(): vector<Simbolo>() {}

Token TabelaDeSimbolos::insert(Tipo tipo) {		// Insere token
	static long long unsigned int unique_index=0;

	assert(unique_index == this->size());
	
	// Criar token
	Token token(tipo,unique_index);
	unique_index++;
	
	// Criar símbolo
	this->emplace_back();
	
	return token;
}

TabelaDeSimbolos::~TabelaDeSimbolos() {
	for(Simbolo& s: *this)
		s.free();
}

/* Simbolo eh um conjunto de atributos, que podem ser inseridos pelos analisadores lexico, sintatico e semantico
	Exemplo de atributos: valor, tipo (int ou long int?), constante (nunca variou apos inicializacao), etc.  
*/
void Simbolo::insert(Atributo* att) {
	assert(this->count(att->nome)==0);
	(*this)[att->nome] = att;
}	

void Simbolo::free() {
	for(pair<string,Atributo*> p: *this)  
		delete p.second;
}


Atributo::Atributo(string _nome): nome(_nome) {}




