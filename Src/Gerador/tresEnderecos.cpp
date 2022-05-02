#include "tresEnderecos.h"
#include "Gerador.h"

#include "../LourdenTM.h"

#include "../TabelaDeSimbolos/TabelaDeSimbolos.h"


using namespace Addr3;
using namespace std;

vector<Assembly*> Instrucao::gera_codigo() {
	return vector<Assembly>();
}

// Declaracao ==================================
vector<Assembly*> Declaracao::gera_codigo() {
	
}	

// Operacao ====================================
vector<Assembly*> Adicao::gera_codigo() {
	
}	

Adicao::Adicao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}


vector<Assembly*> Multiplicacao::gera_codigo() {

}	

Multiplicacao::Multiplicacao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}

vector<Assembly*> Divisao::gera_codigo() {

}	

Divisao::Divisao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}

vector<Assembly*> Subtracao::gera_codigo() {

}	

Subtracao::Subtracao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}

// Saltos ======================================
vector<Assembly*> Salto::gera_codigo() {
	
}
Salto::Salto(long int _linha): linha(_linha) {}
	
// Saltos condicionais =========================

vector<Assembly*> Beq::gera_codigo() {
	
}
Beq::Beq(long int _linha): SaltoCondicional(_linha) {}





