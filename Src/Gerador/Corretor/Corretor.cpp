#include "Corretor.h"

using namespace std;
using namespace Addr3;

Corretor::Corretor(list<shared_ptr<Instrucao> >& _instrucoes): instrucoes(_instrucoes) {}

void Corretor::corrigir() {
	for(auto it=instrucoes.begin(); it!=instrucoes.end(); it++) {
		atual=it;
		(*it)->acao(*this);
	}
}
