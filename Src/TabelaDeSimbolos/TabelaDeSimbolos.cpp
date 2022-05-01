#include "TabelaDeSimbolos.h"
#include <cassert>

using namespace std;


/* A tabela de símbolos é um vetor de conjuntos de atributos, com nome em forma de string*/
TabelaDeSimbolos& TabelaDeSimbolos::getInstance() {	// Caso seja usada como Singleton
	static TabelaDeSimbolos instance;
	return instance;
}
TabelaDeSimbolos::TabelaDeSimbolos() {}

Simbolo& TabelaDeSimbolos::operator[](Token& token) {
	return this->container[token.id()];
}
	
long long unsigned int TabelaDeSimbolos::size() {
	return this->container.size();
}

Token TabelaDeSimbolos::insert(int tipo) {		// Insere token
	static long long unsigned int unique_index=0;

	assert(unique_index == this->container.size());
	
	// Criar token
	Token token(tipo,unique_index);
	unique_index++;
	
	// Criar símbolo
	this->container.emplace_back();
	
	return token;
}

TabelaDeSimbolos::~TabelaDeSimbolos() {
	for(Simbolo& s: this->container)
		s.free();
}

/* Simbolo eh um conjunto de atributos, que podem ser inseridos pelos analisadores lexico, sintatico e semantico
	Exemplo de atributos: valor, tipo (int ou long int?), constante (nunca variou apos inicializacao), etc.  
*/
void Simbolo::insert(Atributo* att) {
	assert(this->container.count(att->nome)==0);
	this->container[att->nome] = att;
}	

Atributo* Simbolo::operator[](std::string key) {
	if(this->container.count(key)==0)
		return nullptr;
	return this->container[key];
}

bool Simbolo::has(std::string key) {
	return this->container.count(key)>0;
}

void Simbolo::free() {
	for(pair<string,Atributo*> p: this->container)  
		delete p.second;
}


Atributo::Atributo(string _nome): nome(_nome) {}

Atributo::~Atributo() {}




