#include "Gerador.h"

#include <vector>

using namespace std;

Gerador::Gerador(vector<Instrucao*> v) {

}

Instrucao::Instrucao(Token _e1,Token _e2,Token _e3): e1(_e1),e2(_e2),e3(_e3) {}


vector<InstrucaoRV> Instrucao::gera_codigo() {
	
}
