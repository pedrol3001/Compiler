#ifndef Sintatico_h
#define Sintatico_h

#include "../Lexico/Lexico.h"
#include "bison.h"

struct Sintatico {
	Sintatico(Lexico &lexico);
	int status;
};


#endif
