#include "Sintatico.h"

#include "parser.tab.c"

Sintatico::Sintatico(Lexico &lexico) {
	status = yyparse(lexico);


}
