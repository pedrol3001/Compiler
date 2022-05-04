#ifndef Sintatico_h
#define Sintatico_h

#include "../Lexico/Lexico.h"
#include "../Semantico/Semantico.h"

#include <memory>

struct Sintatico {	
	bool analisar(Lexico &lexico, Semantico& semantico);
	bool good();
	private:
		bool ok=false;
};


#endif
