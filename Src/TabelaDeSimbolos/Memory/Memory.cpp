#include "Memory.h"
#include <cassert>
#include <string>

using namespace std;

Variavel::Variavel(string _nome, int _size): Atributo(_nome), size(_size) {}

Variavel::~Variavel() {}

long int Variavel::offset() {return offset_;}

void Variavel::setDist(long int _distance) {distance=_distance;}
void Variavel::setOffset(long int pointer) {offset_=pointer-distance;}


VarLocal::VarLocal(int _size): Variavel("VarLocal",_size) {}

VarGlobal::VarGlobal(int _size): Variavel("VarGlobal",_size) {}
