#ifndef TabelaDeSimbolos_Teste_h
#define TabelaDeSimbolos_Teste_h

#include "../TabelaDeSimbolos.h"
#include <string>

struct StrAtt : public Atributo
{
	std::string str;
	StrAtt(std::string _str);
};

struct NameAtt : public Atributo
{
	std::string str;
	NameAtt(std::string _str);
};

#endif
