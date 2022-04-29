#include "Gerador.h"

#include <vector>
#include <string>

using namespace std;

Gerador::Gerador(vector<Instrucao*> v) {

}

Instrucao::Instrucao(Token _e1,Token _e2,Token _e3): e1(_e1),e2(_e2),e3(_e3) {}


vector<Assembly> Instrucao::gera_codigo() {
	vector<Assembly> v;
	return v;
}

Assembly::Assembly(string _nome): nome(_nome) {}
