#include "Gerador.h"

#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "tresEnderecos/tresEnderecos.h"
#include "Assembly/Assembly.h"
#include "Corretor/Corretor.h"

using namespace std;
using namespace Addr3;

Gerador::Gerador(bool _oI, bool _oM): oI(_oI), oM(_oM) {}

list<std::shared_ptr<Assembly> > Gerador::codigo() {
	assert(ok);
	return codigoGerado;
}

bool Gerador::gerar(std::ostream& dst_stream, std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes) {
	instrucoes.emplace_front(new SetZero);	
	
	if(oI) otimizarIntermediario(instrucoes);
	
	corrigir(instrucoes);
	
	codigoGerado.clear();	// limpar antes de usra
	for(shared_ptr<Instrucao> instrucao: instrucoes)  {
		list<shared_ptr<Assembly> > codigo = instrucao->gera_codigo();
		codigoGerado.insert(codigoGerado.end(),codigo.begin(),codigo.end());
	}
	
	if(oM) otimizarTmLourden(codigoGerado);
	
	
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

void Gerador::corrigir(list<shared_ptr<Addr3::Instrucao> >& instrucoes) {
	Corretor corretor(instrucoes);
	corretor.corrigir();
}

void Gerador::otimizarIntermediario(list<shared_ptr<Addr3::Instrucao> >& instrucoes) {
	// nada
}

void Gerador::otimizarTmLourden(list<shared_ptr<Assembly> >& codigo) {
	// nada
}
