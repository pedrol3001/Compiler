#include "tresEnderecos.h"

#include "../LourdenTM/LourdenTM.h"

#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <cassert>


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
list<Assembly*> Aloca::gera_codigo() {
	list<Assembly*>  code;
	// Aloca espaco na pilha
	return code;
}	
Aloca::Aloca(int _bytes): Instrucao(0), bytes(_bytes) {}

list<Assembly*> Desaloca::gera_codigo() {
	list<Assembly*>  code;
	// Desaloca espaco na pilha
	return code;
}	
Desaloca::Desaloca(int _bytes): Instrucao(0), bytes(_bytes) {}

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
	
void loadTemp(list<Assembly*>& code, Token op,TM::Reg reg) {
	TabSim &ts = TabSim::getInstance();
	
	assert(ts[op].has("VarGlobal") || ts[op].has("VarEstatica") || ts[op].has("IntVal"));
	assert(!(ts[op].has("VarGlobal")&& ts[op].has("VarEstatica")));
	assert(!(ts[op].has("VarEstatica")&& ts[op].has("IntVal")));
	assert(!(ts[op].has("IntVal")&& ts[op].has("VarGlobal")));
	
	const long int loadl = Instrucao::linha();
	if(ts[op].has("IntVal")) {
		long long int imm =  ((IntVal*)ts[op]["IntVal"])->val;
		code.push_back(new TM::LDC(reg,imm,reg,loadl));			// loadl: LDC reg,val,reg 
	}
	if(ts[op].has("VarGlobal")) {
		long int offset = ((VarGlobal*)ts[op]["VarGlobal"])->offset();
		code.push_back(new TM::LD(reg,offset,TM::gp,loadl));			// loadl: LD reg,-offset(gp)
	}
	if(ts[op].has("VarEstatica")) {
		long int offset = ((VarEstatica*)ts[op]["VarEstatica"])->offset();
		code.push_back(new TM::LD(reg,offset,TM::sp,loadl));			// loadl: LD reg,-offset(sp)
	}
}

void storeTemp(list<Assembly*>& code, Token op,TM::Reg reg) {
	TabSim &ts = TabSim::getInstance();
	
	assert(ts[op].has("VarGlobal") || ts[op].has("VarEstatica"));
	assert(!(ts[op].has("VarGlobal")&& ts[op].has("VarEstatica")));
	
	const long int storel = Instrucao::linha();
	
	if(ts[op].has("VarGlobal")) {
		long int offset = ((VarGlobal*)ts[op]["VarGlobal"])->offset();
		code.push_back(new TM::ST(reg,offset,TM::gp,storel));				// storel: ST reg,-offset(gp)
	}
	if(ts[op].has("VarEstatica")) {
		long int offset = ((VarEstatica*)ts[op]["VarEstatica"])->offset();
		code.push_back(new TM::ST(reg,offset,TM::sp,storel));				// storel: ST reg,-offset(sp)
	}
}
	
// Adicao	
list<Assembly*> Adicao::gera_codigo() {
	list<Assembly*>  code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza adicao, no temp r2
	const long int addl = Instrucao::linha();
	code.push_back(new TM::ADD(TM::r2,TM::r0,TM::r1,addl));	// addl: ADD r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
	
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
Atribuicao::Atribuicao(Token _dst, Token _op): Operacao(_dst,_op) {}

list<Assembly*> Atribuicao::gera_codigo() {
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

list<Assembly*> Blt::gera_codigo() {
	list<Assembly*>  code;
	// 
	return code;
}
Blt::Blt(Token _op1, Token _op2, long int _instrucao): SaltoCondicional(_instrucao), op1(_op1), op2(_op2) {}





