#include "Sintatico.h"

#include "bison.c"

using namespace std;

Sintatico::Sintatico(Lexico &lexico): vector<Bloco*>() {
	status = yyparse(lexico);


}

Sintatico::~Sintatico() {
	for(Bloco* bloco: *this)
		delete bloco;
}
