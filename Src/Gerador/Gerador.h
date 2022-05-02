#ifndef generator_h
#define generator_h

#include "../Token/Token.h"

#include <vector>
#include <string>

#include "tresEnderecos.h"
#include "LourdenTM.h"
#include "assemblyRiscV.h"

class Gerador {
	Gerador(const std::vector<Addr3::Instrucao*> instrucoes, int otimizar=0);
	
	//void otimizar();
};


#endif
