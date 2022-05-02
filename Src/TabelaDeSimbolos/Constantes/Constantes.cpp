#include "Constantes.h"

#include <string>

using namespace std;


IntVal::IntVal(string _val) : Atributo("IntVal"), val(stoll(_val,0,10)) {}

DoubleVal::DoubleVal(string _val) : Atributo("DoubleVal"), val(stold(_val)) {}

IdVal::IdVal(string _str) : Atributo("IdVal"), val(_str) {}

StringVal::StringVal(std::string _str): Atributo("StringVal"), val(_str) {}
