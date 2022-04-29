#ifndef generator_h
#define generator_h

#include "../Token/Token.h"

#include <vector>
#include <string>

class Instrucao;
class Assembly;

class Gerador {
	Gerador(std::vector<Instrucao*> v);
	
	//void otimizar();
};


class Instrucao {
	Token e1,e2,e3;	// 3 endereços
	
	public:
	Instrucao(Token _e1,Token _e2,Token _e3);
	std::vector<Assembly> gera_codigo();
};

struct Assembly {
	std::string nome;
	Assembly(std::string _nome);
	virtual std::string toStr()=0;
};

#include "assemblyRiscV.h"
#include "tres_enderecos.h"

#endif
