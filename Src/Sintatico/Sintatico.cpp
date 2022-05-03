#include "Sintatico.h"

#include "../bison.c"

using namespace std;

Sintatico::Sintatico(Lexico &lexico) {
	int status = yyparse(lexico,container);
	ok = (status==0);

}

bool Sintatico::good() {
	return ok;
}

Sintatico::~Sintatico() {
	for(Bloco* bloco: this->container)
		delete bloco;
}
