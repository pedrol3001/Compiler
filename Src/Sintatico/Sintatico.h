#ifndef Sintatico_h
#define Sintatico_h

#include "../Lexico/Lexico.h"
#include "parser.tab.h"

struct Sintatico {
	Sintatico(Lexico &lexico);
	int status;
};


#endif
