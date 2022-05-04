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
	
		void otimizarIntermediario();
		void otimizarTmLourden();
		
		std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes;
		std::list<std::shared_ptr<Assembly> > codigoGerado;
};


#endif
