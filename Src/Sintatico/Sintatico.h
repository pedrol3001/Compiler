#ifndef Sintatico_h
#define Sintatico_h

#include "../Lexico/Lexico.h"
#include "../Semantico/Semantico.h"

struct Sintatico {
	std::vector<Bloco*> container;
	Sintatico(Lexico &lexico);
	~Sintatico();
	int status;
};


#endif
