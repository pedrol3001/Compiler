#include "tresEnderecos.h"

#include "../LourdenTM/LourdenTM.h"

#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"


using namespace Addr3;
using namespace std;

long long int Instrucao::count=-1;
long long int Instrucao::linha() {
	Instrucao::count++;
	return Instrucao::count;
}

Addr3::Instrucao::Instrucao(int _size): size(_size) {}
Addr3::Instrucao::~Instrucao() {}

// Declaracao ==================================
list<Assembly*> Empilha::gera_codigo() {
	list<Assembly*>  code;
	// Aloca espaco na pilha
	return code;
}	
Empilha::Empilha(Token _token): Instrucao(0) {}

list<Assembly*> Desempilha::gera_codigo() {
	list<Assembly*>  code;
	// Desaloca espaco na pilha
	return code;
}	
Desempilha::Desempilha(Token _token): Instrucao(0) {}

// Operacao ====================================
Operacao::~Operacao() {}
Operacao::Operacao(Token _dst, vector<Token>& _op): dst(_dst), op(_op), Instrucao(0) {}
Operacao::Operacao(Token _dst, Token _op): dst(_dst), Instrucao(0) {
	op.push_back(_op);
}	
Operacao::Operacao(Token _dst, Token _op1, Token _op2): dst(_dst), Instrucao(0) {
	op.push_back(_op1);
	op.push_back(_op2);
}		
	
// Adicao	
list<Assembly*> Adicao::gera_codigo() {
	list<Assembly*>  code;
	// Realiza adicao, carregando operandos da pilha e salvando resultado na pilha
	return code;
}	
Adicao::Adicao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Multiplicacao
list<Assembly*> Multiplicacao::gera_codigo() {
	list<Assembly*> code;
	// Realiza multiplicacao, carregando operandos da pilha e salvando resultado na pilha
	return code;
}	

Multiplicacao::Multiplicacao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Divisao
list<Assembly*> Divisao::gera_codigo() {
	list<Assembly*>  code;
	// Realiza divisao, carregando operandos da pilha e salvando resultado na pilha
	return code;
}	
Divisao::Divisao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Subtracao
list<Assembly*> Subtracao::gera_codigo() {
	list<Assembly*> code;
	// Realiza subtracao, carregando operandos da pilha e salvando resultado na pilha
	
	return code;
}	

Subtracao::Subtracao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Assign
Assignment::Assignment(Token _dst, Token _op): Operacao(_dst,_op) {}

list<Assembly*> Assignment::gera_codigo() {
	list<Assembly*> code;
	// Realiza atribuicao, carregando operandos da pilha e salvando resultado na pilha
	
	
	return code;
}	


// Saltos ======================================
list<Assembly*> Salto::gera_codigo() {
	list<Assembly*>  code;
	
	return code;
}
Salto::Salto(long int _instrucao): instrucao(_instrucao), Instrucao(0) {}
	
// Saltos condicionais =========================
SaltoCondicional::SaltoCondicional(long int _instrucao): instrucao(_instrucao), Instrucao(0) {}

list<Assembly*> Beq::gera_codigo() {
	list<Assembly*>  code;
	// 
	return code;
}
Beq::Beq(long int _instrucao): SaltoCondicional(_instrucao) {}





