#include "Memory.h"
#include <cassert>
#include <string>

using namespace std;

Variavel::Variavel(string _nome, long int _offset, int _bytes): Atributo(_nome), offset_(_offset), bytes(_bytes) {}

Variavel::~Variavel() {}

long int Variavel::offset() {return offset_;}


VarEstatica::VarEstatica(long int _offset, int _bytes): Variavel("VarEstatica",_offset,_bytes) {}

VarGlobal::VarGlobal(long int _offset, int _bytes): Variavel("VarGlobal",_offset,_bytes) {}
