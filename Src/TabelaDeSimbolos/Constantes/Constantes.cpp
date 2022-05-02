#include "Constantes.h"

#include <string>

using namespace std;

char *aux;
IntVal::IntVal(string _val) : Atributo("IntVal"), val(strtoll(_val.c_str(), &aux, 0)) {}

DoubleVal::DoubleVal(string _val) : Atributo("DoubleVal"), val(stold(_val.c_str())) {}

IdVal::IdVal(string _str) : Atributo("IdVal"), str(_str) {}
