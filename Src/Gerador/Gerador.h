#ifndef generator_h
#define generator_h

#include "../Token/Token.h"

#include <vector>
#include <string>

class Instrucao;
class InstrucaoRV;

class Gerador {
	Gerador(std::vector<Instrucao*> v);
	
	//void otimizar();
};


class Instrucao {
	Token e1,e2,e3;	// 3 endereços
	
	public:
	Instrucao(Token _e1,Token _e2,Token _e3);
	std::vector<InstrucaoRV> gera_codigo();
};

enum RegRV {
	zero,
	ra,sp,gp,tp,
	t0,t1,t2,t3,t4,t5,t6,
	s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,
	a0,a1,a2,a3,a4,a5,a6,a7,
	pc	// nao use
};

struct InstrucaoRV {
	std::string nome;
	virtual std::string toStr()=0;
};

#include "tres_enderecos.h"

#endif
