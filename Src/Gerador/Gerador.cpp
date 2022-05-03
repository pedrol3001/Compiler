#include "Gerador.h"

#include <list>
#include <string>
#include <iostream>
#include <cassert>

#include "tresEnderecos/tresEnderecos.h"
#include "LourdenTM/LourdenTM.h"
#include "assemblyRiscV/assemblyRiscV.h"

using namespace std;
using namespace Addr3;

Gerador::Gerador(bool _oI, bool _oM): oI(_oI), oM(_oM) {}

list<std::shared_ptr<Assembly> > Gerador::codigo() {
	assert(ok);
	return codigoGerado;
}

bool Gerador::gerar(std::ostream& dst_stream, std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes) {
	codigoGerado.clear();
	
	
	if(oI) {
		otimizarIntermediario();
		corrigirIntermediario();
	}
	
	for(shared_ptr<Instrucao> instrucao: instrucoes) 
		gerarInstrucaoTM(instrucao);
	
	if(oM) {
		otimizarTmLourden();
		corrigirTmLourden();
	}
	
	ok = true;
	return ok;
}

void Gerador::otimizarIntermediario() {
	// nada
}
void Gerador::corrigirIntermediario() {
	// nada
}

void Gerador::gerarInstrucaoTM(shared_ptr<Instrucao> instrucao) {
	// nada
}

void Gerador::otimizarTmLourden() {
	// nada
}
void Gerador::corrigirTmLourden() {
	// nada
}
