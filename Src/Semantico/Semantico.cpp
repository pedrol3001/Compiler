#include "Semantico.h"
#include "../Sintatico/Sintatico.h"
#include "../Util/Util.h"

#include <cassert>

using namespace std;
using namespace Addr3;


Semantico::Semantico(): tabela(), escopo(tabela.escopo){
	
}

bool Semantico::analisar() {
	ok = true;	// ok eh o resultado da analise
	
	//gerarIntermediario();
	
	return ok;
}

void Semantico::gerarIntermediario() {
	assert(ok);
	
	code.clear();
	
	// Geracao de codigo intermediario
	// TODO: é um teste
}
#include <iostream>
std::list<std::shared_ptr<Addr3::Instrucao> > Semantico::instrucoes() {
	assert(ok);
	return code;
}

bool Semantico::good() {return ok;}
