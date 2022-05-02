#ifndef Semantico_h
#define Semantico_h

#include "Bloco.h"	// Olhar esse .h

#include <vector>

class Sintatico;	// Definido em Sintatico.h

struct Semantico {
	Semantico();
	Semantico(Sintatico &sintatico);
};

struct State {
	
	
};

//enum tipo { _int, _float, _void };


#endif
