#include "Semantico.h"
#include "../Sintatico/Sintatico.h"
#include "../Util/Util.h"

#include <cassert>

using namespace std;
using namespace Addr3;

bool Semantico::analisar(std::vector<std::shared_ptr<Bloco> > blocos) {
	
	ok = true;	// ok eh o resultado da analise
	
	gerarIntermediario();
	
	return ok;
}

void Semantico::gerarIntermediario() {
	assert(ok);
	
	instrucoesGeradas.clear();
	
	// Geracao de codigo intermediario
}

std::list<std::shared_ptr<Addr3::Instrucao> > Semantico::instrucoes() {
	assert(ok);
	return instrucoesGeradas;
}

bool Semantico::good() {return ok;}
