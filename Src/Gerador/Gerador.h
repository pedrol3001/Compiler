#ifndef generator_h
#define generator_h

#include "../Token/Token.h"

#include <list>
#include <string>
#include <ostream>

#include <memory>

#include "tresEnderecos/tresEnderecos.h"
#include "LourdenTM/LourdenTM.h"
#include "Assembly/Assembly.h"

struct Gerador {
	Gerador(bool _oI=false, bool _oM=false);
	bool gerar(std::ostream& dst_stream, std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes);
	std::list<std::shared_ptr<Assembly> > codigo();
	
	private:
		bool oI=false,oM=false;
		bool ok=false;
	
		void corrigir(std::list<std::shared_ptr<Addr3::Instrucao> >& instrucoes);
		
		void otimizarIntermediario(std::list<std::shared_ptr<Addr3::Instrucao> >& instrucoes);
		void otimizarTmLourden(std::list<std::shared_ptr<Assembly> >& codigo);
		
		std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes;
		std::list<std::shared_ptr<Assembly> > codigoGerado;
};

#endif
