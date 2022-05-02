#include "tresEnderecos.h"

#include "LourdenTM.h"

#include "../TabelaDeSimbolos/TabelaDeSimbolos.h"


using namespace Addr3;
using namespace std;

Addr3::Instrucao::Instrucao(int _size): size(_size) {}

// Declaracao ==================================
vector<Assembly*> Declaracao::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}	
Declaracao::Declaracao(Token _token): Instrucao(0) {}

// Operacao ====================================
Operacao::Operacao(Token _dst, vector<Token>& _op): dst(_dst), op(_op), Instrucao(0) {}	
		
vector<Assembly*> Adicao::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}	

Adicao::Adicao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}


vector<Assembly*> Multiplicacao::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}	

Multiplicacao::Multiplicacao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}

vector<Assembly*> Divisao::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}	

Divisao::Divisao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}

vector<Assembly*> Subtracao::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}	

Subtracao::Subtracao(Token _dst, vector<Token>& op): Operacao(_dst,op) {}

// Saltos ======================================
vector<Assembly*> Salto::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}
Salto::Salto(long int _linha): linha(_linha), Instrucao(0) {}
	
// Saltos condicionais =========================
SaltoCondicional::SaltoCondicional(long int _linha): linha(_linha), Instrucao(0) {}

vector<Assembly*> Beq::gera_codigo() {
	vector<Assembly*>  v;
	return v;
}
Beq::Beq(long int _linha): SaltoCondicional(_linha) {}





