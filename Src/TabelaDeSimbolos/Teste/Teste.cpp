#include "Teste.h"

#include <string>

using namespace std;

StrAtt::StrAtt(string _str, long long int _linha, long long int _coluna, long long int _posicao) : 
	Atributo("StrAtt"), str(_str),linha(_linha),coluna(_coluna),posicao(_posicao) {}

NameAtt::NameAtt(string _str) : Atributo("NameAtt"), str(_str) {}
