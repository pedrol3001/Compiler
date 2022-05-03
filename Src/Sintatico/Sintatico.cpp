#include "Sintatico.h"
#include "../Util/Util.h"

#include "../bison.c"

#include <cassert>

using namespace std;

bool Sintatico::analisar(Lexico &lexico) {
	blocosGerados.clear();	
	int status = yyparse(lexico,blocosGerados);
	ok = (status==0);
	return ok;
}

std::vector<std::shared_ptr<Bloco> > Sintatico::blocos() {
	assert(ok);
	return blocosGerados;
}
	
bool Sintatico::good() {return ok;}
