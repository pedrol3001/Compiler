#ifndef StackReference_h
#define StackReference_h

#include "../TabelaDeSimbolos.h"
#include "../../Token/Token.h"

#include <string>

struct Variavel: public Atributo {
	long int offset_;
	long int size;
	
	Variavel(std::string _nome, long int _offset, int _size);
	virtual ~Variavel();
	
	long int offset();
};

struct VarLocal: public Variavel {	// Variaveis estaticas
	VarLocal(long int _offset, int _size);	// space = 
};

struct VarGlobal: public Variavel {	// Variaveis globais
	VarGlobal(long int _offset, int _size);	// space = 
};

#endif
