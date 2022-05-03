#ifndef generator_h
#define generator_h

#include "../Token/Token.h"

#include <list>
#include <string>
#include <ostream>

#include <memory>

#include "tresEnderecos/tresEnderecos.h"
#include "LourdenTM/LourdenTM.h"

struct Gerador {
	Gerador(bool _oI=false, bool _oM=false);
	bool gerar(std::ostream& dst_stream, std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes);
	std::list<std::shared_ptr<TM::Inst> > codigo();
	
	private:
		bool oI=false,oM=false;
		bool ok=false;
	
		void otimizarIntermediario();
		void corrigirIntermediario();
		
		void gerarInstrucaoTM(std::shared_ptr<Addr3::Instrucao> instrucao);
		
		void otimizarTmLourden();
		void corrigirTmLourden();
		
		std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes;
		std::list<std::shared_ptr<TM::Inst> > codigoGerado;
};


#endif
