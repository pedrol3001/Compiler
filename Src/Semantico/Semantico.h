#ifndef Semantico_h
#define Semantico_h

#include "Bloco/Bloco.h"	// Olhar esse .h
#include "../Gerador/Gerador.h"
#include "../Sintatico/Sintatico.h"

#include <memory>
#include <list>
#include <vector>

struct Semantico {
	bool analisar(std::vector<std::shared_ptr<Bloco> > blocos);
	std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes();
	bool good();
	private:
		void gerarIntermediario();
		
		std::list<std::shared_ptr<Addr3::Instrucao> > instrucoesGeradas;
		bool ok=false;
};

struct State {
	
	
};

//enum tipo { _int, _float, _void };


#endif
