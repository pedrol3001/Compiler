#ifndef StackReference_h
#define StackReference_h

#include "../TabelaDeSimbolos.h"
#include "../../Token/Token.h"

#include <string>

struct Variavel: public Atributo {
	long int offset_;
	long int bytes;
	
	Variavel(std::string _nome, long int _offset, int _bytes);
	virtual ~Variavel();
	
	long int offset();
};

struct VarLocal: public Variavel {	// Variaveis estaticas
	VarLocal(long int _offset, int _bytes);	// space = 
};

struct VarGlobal: public Variavel {	// Variaveis globais
	VarGlobal(long int _offset, int _bytes);	// space = 
};

#endif
