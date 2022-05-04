#ifndef Jump_h
#define Jump_h

#include "../TabelaDeSimbolos.h"

struct LabelVal: Atributo {
	void setLinha(long long int _linha);
	long long int getLinha();
	LabelVal();
	private:
		long long int linha=-1;
};

#endif
