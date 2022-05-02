#include "Sintatico.h"

#include "../bison.c"

using namespace std;

Sintatico::Sintatico(Lexico &lexico) {
	status = yyparse(lexico,container);


}

Sintatico::~Sintatico() {
	for(Bloco* bloco: this->container)
		delete bloco;
}
