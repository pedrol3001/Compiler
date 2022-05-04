#include "Gerador.h"

#include <list>
#include <string>
#include <iostream>
#include <cassert>

#include "tresEnderecos/tresEnderecos.h"
#include "Assembly/Assembly.h"

using namespace std;
using namespace Addr3;

Gerador::Gerador(bool _oI, bool _oM): oI(_oI), oM(_oM) {}

list<std::shared_ptr<Assembly> > Gerador::codigo() {
	assert(ok);
	return codigoGerado;
}

bool Gerador::gerar(std::ostream& dst_stream, std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes) {
	codigoGerado.clear();
	
	
	if(oI) otimizarIntermediario();
	
	for(shared_ptr<Instrucao> instrucao: instrucoes)  {
		list<shared_ptr<Assembly> > codigo = instrucao->gera_codigo();
		codigoGerado.insert(codigoGerado.end(),codigo.begin(),codigo.end());
	}
	
	if(oM) otimizarTmLourden();
	
	substituirLabels();
	
	ok = true;
	return ok;
}

void Gerador::otimizarIntermediario() {
	// nada
}

void Gerador::otimizarTmLourden() {
	// nada
}
void Gerador::substituirLabels() {
	// nada
}
