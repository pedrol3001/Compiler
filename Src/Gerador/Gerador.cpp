#include "Gerador.h"

#include <list>
#include <string>
#include <iostream>
#include <iomanip>
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
	
	setOffsets();
	
	if(oM) otimizarTmLourden();
	
	// Atualiza labels
	long long int linha=0;
	for(shared_ptr<Assembly> assembly: codigoGerado) {
		assembly->update(linha);
		if(!assembly->ignore())
			linha++;
	}
	
	// Gera codigo
	linha = 0;
	for(shared_ptr<Assembly> assembly: codigoGerado) {
		if(assembly->ignore())
			dst_stream << "*" << assembly->str() << '\n'; 
		else
			dst_stream << linha++ << ": " << setw(3) << assembly->str() << '\n';
	}
	
	return ok = true;
}

void Gerador::setOffsets() {
	for(shared_ptr<Instrucao> instrucao: instrucoes)  {
	}
}

void Gerador::otimizarIntermediario() {
	// nada
}

void Gerador::otimizarTmLourden() {
	// nada
}
