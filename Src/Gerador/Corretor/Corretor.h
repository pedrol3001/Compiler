#ifndef Corretor_h
#define Corretor_h

#include "../tresEnderecos/tresEnderecos.h"

#include <list>
#include <memory>

struct Corretor {
	Corretor(std::list<std::shared_ptr<Addr3::Instrucao> >& _instrucores);
	void corrigir();
	public:
		long long int param=0;
		long long int sp=0,gp=0;
		std::list<std::shared_ptr<Addr3::Instrucao> >::iterator atual;
		std::list<std::shared_ptr<Addr3::Instrucao> >& instrucoes;
};


#endif
