#ifndef TabelaDeSimbolos_Teste_h
#define TabelaDeSimbolos_Teste_h

#include "../TabelaDeSimbolos.h"
#include <string>

struct StrAtt : public Atributo
{
	std::string str;
	long long int linha,coluna,posicao;
	StrAtt(std::string _str, long long int _linha=-1, long long int _coluna=-1, long long int _posicao=-1);
};

struct NameAtt : public Atributo
{
	std::string str;
	NameAtt(std::string _str);
};

#endif
