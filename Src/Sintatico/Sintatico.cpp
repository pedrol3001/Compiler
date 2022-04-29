#include "Sintatico.h"

#include "bison.c"

Sintatico::Sintatico(Lexico &lexico) {
	status = yyparse(lexico);


}
