#ifndef StackReference_h
#define StackReference_h

#include "../TabelaDeSimbolos.h"
#include "../../Token/Token.h"

#include <string>

struct Variavel: public Atributo {
	static long int *counter;
	long int distance;
	long int bytes;
	
	Variavel(std::string _nome, long int *_counter, int _bytes);
	virtual ~Variavel();
	
	static void free(long int _bytes);
	void free();
	long int getOffset();
};

struct VarEstatica: public Variavel {	// Variaveis estaticas
	static long int sp_counter;
	VarEstatica(int _bytes);	// space = 
};

struct VarGlobal: public Variavel {	// Variaveis globais
	static long int gp_counter;
	VarGlobal(int _bytes);	// space = 
};

#endif
