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

Addr3::Instrucao::Instrucao(): size(0) {}
Addr3::Instrucao::~Instrucao() {}

// Declaracao ==================================
list<Assembly*> Aloca::gera_codigo() {
	list<Assembly*>  code;
	// Carregar constante
	const long int ctel = Instrucao::linha();
	code.push_back(new TM::LDC(TM::r0,space,TM::r0,ctel));		// allocl: LDC r0,space(r0)	
	// Aloca espaco na pilha
	const long int allocl = Instrucao::linha();
	code.push_back(new TM::ADD(TM::sp,TM::sp,TM::r0,allocl));	// allocl: ADD sp,sp,r0	
	
	size = code.size();	
	return code;
}	
Aloca::Aloca(int _space): space(_space) {}

list<Assembly*> Desaloca::gera_codigo() {
	list<Assembly*>  code;
	// Carregar constante
	const long int ctel = Instrucao::linha();
	code.push_back(new TM::LDC(TM::r0,space,TM::r0,ctel));		// allocl: LDC r0,space(r0)	
	// Desaloca espaco na pilha
	const long int deallocl = Instrucao::linha();
	code.push_back(new TM::SUB(TM::sp,TM::sp,TM::r0,deallocl));	// deallocl: SUB sp,sp,r0
		
	size = code.size();	
	return code;
}	
Desaloca::Desaloca(int _space): space(_space) {}

// Operacao ====================================
Operacao::~Operacao() {}
Operacao::Operacao(Token _dst, vector<Token>& _op): dst(_dst), op(_op) {}
Operacao::Operacao(Token _dst, Token _op): dst(_dst) {
	op.push_back(_op);
}	
Operacao::Operacao(Token _dst, Token _op1, Token _op2): dst(_dst) {
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
	
	size = code.size();	
	return code;
}	
Adicao::Adicao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Multiplicacao
list<Assembly*> Multiplicacao::gera_codigo() {
	list<Assembly*> code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza multiplicacao, no temp r2
	const long int mull = Instrucao::linha();
	code.push_back(new TM::MUL(TM::r2,TM::r0,TM::r1,mull));	// mull: MUL r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
	
	size = code.size();	
	return code;
}	

Multiplicacao::Multiplicacao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Divisao
list<Assembly*> Divisao::gera_codigo() {
	list<Assembly*>  code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza divisao, no temp r2
	const long int divl = Instrucao::linha();
	code.push_back(new TM::DIV(TM::r2,TM::r0,TM::r1,divl));	// divl: DIV r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
	
	size = code.size();	
	return code;
}	
Divisao::Divisao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Subtracao
list<Assembly*> Subtracao::gera_codigo() {
	list<Assembly*> code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza divisao, no temp r2
	const long int subl = Instrucao::linha();
	code.push_back(new TM::SUB(TM::r2,TM::r0,TM::r1,subl));	// subl: SUB r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
	
	size = code.size();	
	return code;
}	

Subtracao::Subtracao(Token _dst, Token _op1, Token _op2): Operacao(_dst,_op1,_op2) {}

// Assign
Atribuicao::Atribuicao(Token _dst, Token _op): Operacao(_dst,_op) {}

list<Assembly*> Atribuicao::gera_codigo() {
	list<Assembly*> code;
	// Carregar operando op[0]
	loadTemp(code,op[0],TM::r0);
	// Realiza atribuicao, no temp r2
	storeTemp(code,dst,TM::r0);	// dst = op[0]
	
	size = code.size();	
	return code;
}	


// Saltos ======================================
list<Assembly*> Salto::gera_codigo() {
	list<Assembly*>  code;
	/*
	
	// Carregar constante
	const long int ctel = Instrucao::linha();
	code.push_back(new TM::LDC(TM::r0,0,TM::r0,ctel));		// allocl: LDC r0,0(r0)	
	const long int jeql = Instrucao::linha();
	code.push_back(new TM::JEQ(TM::r0,distancia,TM::pc,jeql));	// jeql: JEQ r0,distancia(pc)
	*/
	size = code.size();	
	return code;
}
Salto::Salto(long int _linha): linha(_linha) {}
	
// Saltos condicionais =========================
SaltoCondicional::SaltoCondicional(long int _linha): linha(_linha) {}

list<Assembly*> Beq::gera_codigo() {
	list<Assembly*>  code;
	/*
	
	// Carregar operandos (op1, op2)
	loadTemp(code,op1,TM::r0);
	loadTemp(code,op2,TM::r1);
	// Subtrair
	const long int subl = Instrucao::linha();
	code.push_back(new TM::SUB(TM::r2,TM::r0,TM::r1,subl));	// subl: SUB r2,r0,r1
	const long int jeql = Instrucao::linha();
	code.push_back(new TM::JEQ(TM::r2,distancia,TM::pc,jeql));	// jeql: JEQ r2,distancia(pc)
	*/
	
	size = code.size();	
	return code;
}
Beq::Beq(Token _op1, Token _op2, long int _distancia): SaltoCondicional(_distancia), op1(_op1), op2(_op2) {}





