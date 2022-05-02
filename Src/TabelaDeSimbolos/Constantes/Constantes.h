#ifndef AttConstantes_h
#define AttConstantes_h

#include "../TabelaDeSimbolos.h"

// OBS: Codigo feito por Vitor
struct IntVal : public Atributo
{
	long long int val;
	IntVal(std::string _val);
};

struct DoubleVal : public Atributo
{
	long double val;
	DoubleVal(std::string _val);
};

struct IdVal : public Atributo
{
	std::string str;
	IdVal(std::string _str);
};

#endif
