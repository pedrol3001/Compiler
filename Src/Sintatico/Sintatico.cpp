#include "Sintatico.h"
#include "../Util/Util.h"

#include "../bison.c"

#include <cassert>

using namespace std;

bool Sintatico::analisar(Lexico &lexico, Semantico& semantico) {
	int status = yyparse(lexico,semantico);
	ok = (status==0);
	return ok;
}
	
bool Sintatico::good() {return ok;}
