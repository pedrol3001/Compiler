#ifndef Semantico_h
#define Semantico_h

#include "Bloco/Bloco.h"	// Olhar esse .h
#include "TabelaSemantica/TabelaSemantica.h"
#include "../Gerador/Gerador.h"
#include "../Sintatico/Sintatico.h"


#include <memory>
#include <list>
#include <vector>
#include <string>

struct Semantico {
	Semantico();

	bool analisar();
	std::list<std::shared_ptr<Addr3::Instrucao> > instrucoes();
	bool good();
	void mostrar_analise();
	void salvar_pseudoassembly();
	
	TabelaSemantica tabela;
	TempGenerator tempGen;
	LabelGenerator labelGen;
	int &escopo;
	
	std::list<std::string> pseudoassembly;

	std::list<std::shared_ptr<Addr3::Instrucao> > code;
	
	public:
		std::list<std::shared_ptr<Addr3::Instrucao> > init_code;
	private:
		void gerarIntermediario();
		bool ok=false;
	
};

struct State {
	
	
};

//enum tipo { _int, _float, _void };


#endif
