#ifndef Sintatico_h
#define Sintatico_h

#include "../Lexico/Lexico.h"
#include "../Semantico/Semantico.h"
#include "bison.h"

struct Sintatico: std::vector<Bloco*> {
	Sintatico(Lexico &lexico);
	~Sintatico();
	int status;
};


#endif
