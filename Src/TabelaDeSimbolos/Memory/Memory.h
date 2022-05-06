#ifndef StackReference_h
#define StackReference_h

#include "../TabelaDeSimbolos.h"
#include "../../Token/Token.h"

#include <string>

struct Variavel: public Atributo {
	long int offset_=0;
	long int size;
	
	Variavel(std::string _nome, int _size);
	virtual ~Variavel();
	
	long int offset();
};

struct VarLocal: public Variavel {	// Variaveis estaticas
	VarLocal(int _size);	// space = 
};

struct VarGlobal: public Variavel {	// Variaveis globais
	VarGlobal(int _size);	// space = 
};

#endif
