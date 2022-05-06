#include "Memory.h"
#include <cassert>
#include <string>

using namespace std;

Variavel::Variavel(string _nome, long int _offset, int _size): Atributo(_nome), offset_(_offset), size(_size) {}

Variavel::~Variavel() {}

long int Variavel::offset() {return offset_;}


VarLocal::VarLocal(long int _offset, int _size): Variavel("VarLocal",_offset,_size) {}

VarGlobal::VarGlobal(long int _offset, int _size): Variavel("VarGlobal",_offset,_size) {}
