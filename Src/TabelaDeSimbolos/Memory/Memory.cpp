#include "Memory.h"
#include <cassert>
#include <string>

using namespace std;

long int *Variavel::counter = NULL;
Variavel::Variavel(string _nome, long int *_counter, int _bytes): Atributo(_nome), bytes(_bytes) {
	Variavel::counter = _counter;
	assert(Variavel::counter != NULL);
	distance = *counter+bytes;
	*(Variavel::counter) += bytes;
}
Variavel::~Variavel() {}
long int Variavel::getOffset() {
	return *(Variavel::counter) - distance;
}
void Variavel::free(long int _bytes) {
	*(Variavel::counter) -= _bytes;
	assert(*(Variavel::counter)>=0);	
}
void Variavel::free() {
	*(Variavel::counter) -= bytes;
	assert(*(Variavel::counter)>=0);	
}

long int VarEstatica::sp_counter=0;
VarEstatica::VarEstatica(int _bytes): Variavel("VarEstatica",&(VarEstatica::sp_counter),_bytes) {}

long int VarGlobal::gp_counter=0;
VarGlobal::VarGlobal(int _bytes): Variavel("VarGlobal",&(VarGlobal::gp_counter),_bytes) {}
