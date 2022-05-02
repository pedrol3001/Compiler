#ifndef generator_h
#define generator_h

#include "../Token/Token.h"

#include <vector>
#include <string>

class Instrucao;
class Assembly;

class Gerador {
	Gerador(Semantico& semantico);
	
	//void otimizar();
};

struct Assembly {
	std::string nome;
	Assembly(std::string _nome);
	virtual std::string str()=0;
};

#include "LourdenTM.h"
#include "assemblyRiscV.h"
#include "tresEnderecos.h"

#endif
