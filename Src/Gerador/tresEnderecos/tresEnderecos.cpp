#include "tresEnderecos.h"

#include "../LourdenTM/LourdenTM.h"

#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <cassert>

using namespace Addr3;
using namespace std;

Instrucao::Instrucao(std::string _classe, bool _is_label): classe(_classe), Code::Codigo(_is_label) {}
Instrucao::~Instrucao() {}

// Funcoes auxiliares ===========================
	
void loadReg(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg) {
	TabSim &ts = TabSim::getInstance();
	
	assert(ts[op].has("VarGlobal") || ts[op].has("VarLocal") || ts[op].has("IntVal"));
	assert(!(ts[op].has("VarGlobal")&& ts[op].has("VarLocal")));
	assert(!(ts[op].has("VarLocal")&& ts[op].has("IntVal")));
	assert(!(ts[op].has("IntVal")&& ts[op].has("VarGlobal")));
	
	if(ts[op].has("IntVal")) {
		long long int imm =  ((IntVal*)ts[op]["IntVal"])->val;
		code.emplace_back(new TM::LDC(reg,imm,reg));			// loadl: LDC reg,val,reg 
	}
	if(ts[op].has("VarGlobal")) {
		long int offset = ((VarGlobal*)ts[op]["VarGlobal"])->offset();
		code.emplace_back(new TM::LD(reg,offset,TM::gp));			// loadl: LD reg,-offset(gp)
	}
	if(ts[op].has("VarLocal")) {
		long int offset = ((VarLocal*)ts[op]["VarLocal"])->offset();
		code.emplace_back(new TM::LD(reg,offset,TM::sp));			// loadl: LD reg,-offset(sp)
	}
}

void storeReg(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg) {
	TabSim &ts = TabSim::getInstance();
	
	assert(ts[op].has("VarGlobal") || ts[op].has("VarLocal"));
	assert(!(ts[op].has("VarGlobal")&& ts[op].has("VarLocal")));
		
	if(ts[op].has("VarGlobal")) {
		long int offset = ((VarGlobal*)ts[op]["VarGlobal"])->offset();
		code.emplace_back(new TM::ST(reg,offset,TM::gp));				// storel: ST reg,-offset(gp)
	}
	if(ts[op].has("VarLocal")) {
		long int offset = ((VarLocal*)ts[op]["VarLocal"])->offset();
		code.emplace_back(new TM::ST(reg,offset,TM::sp));				// storel: ST reg,-offset(sp)
	}
}

void alocar(list<shared_ptr<Assembly> >& code, int space,TM::Reg temp,TM::Reg pointer) {
	// Carregar constante
	code.emplace_back(new TM::LDC(temp,space,temp));	//  LDC temp,space(temp)	
	// Aloca espaco na pilha
	code.emplace_back(new TM::SUB(pointer,pointer,temp));	//  SUB pointer,pointer,temp		
}

void desalocar(list<shared_ptr<Assembly> >& code, int space,TM::Reg temp,TM::Reg pointer) {
	// Carregar constante
	code.emplace_back(new TM::LDC(temp,space,temp));	//  LDC temp,space(temp)	
	// Desaloca espaco na pilha
	code.emplace_back(new TM::ADD(pointer,pointer,temp));	//  ADD pointer,pointer,temp		
}

// Label ========================================
std::list<std::shared_ptr<Assembly> > Label::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Label(label));		// Label
	return code;
}
Label::Label(Token _label): Code::Label(_label), Instrucao("Label",true) {}

// Input/output =================================
	
list<shared_ptr<Assembly> > Read::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Le input
	code.emplace_back(new TM::IN(TM::t0,TM::t0,TM::t0));		// IN t0,t0,t0	
	// Armazena input	
	storeReg(code,op,TM::t0);
		
	return code;
}	
Read::Read(Token _op): op(_op), Instrucao("Read") {}

list<shared_ptr<Assembly> > Print::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	
	// Carrega operando	
	loadReg(code,op,TM::t0);
	// Escreve output
	code.emplace_back(new TM::OUT(TM::t0,TM::t0,TM::t0));		// OUT t0,t0,t0	
		
	return code;
}	
Print::Print(Token _op): op(_op), Instrucao("Print") {}

// Declaracao ==================================
list<shared_ptr<Assembly> > Global::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	int space = 1;	// normalmente, deveria ser extraido de Token op
	alocar(code,space,TM::t0,TM::gp);		
	return code;
}	
Global::Global(Token _op): op(_op), Instrucao("Global") {}

list<shared_ptr<Assembly> > Aloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	int space = 1;	// normalmente, deveria ser extraido de Token op
	alocar(code,space,TM::t0,TM::sp);		
	return code;
}	
Aloca::Aloca(Token _op): op(_op), Instrucao("Aloca") {}

list<shared_ptr<Assembly> > Desaloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	int space = 1;	// normalmente, deveria ser extraido de Token op
	desalocar(code,space,TM::t0,TM::sp);		
	return code;
}	
Desaloca::Desaloca(Token _op): op(_op), Instrucao("Desaloca") {}

// Operacao ====================================
Operacao::~Operacao() {}
Operacao::Operacao(string _classe, Token _dst, vector<Token>& _op): dst(_dst), op(_op), Instrucao(_classe) {}
Operacao::Operacao(string _classe, Token _dst, Token _op): dst(_dst), Instrucao(_classe) {
	op.emplace_back(_op);
}	
Operacao::Operacao(string _classe, Token _dst, Token _op1, Token _op2): dst(_dst), Instrucao(_classe) {
	op.emplace_back(_op1);
	op.emplace_back(_op2);
}		
	
// Adicao	
list<shared_ptr<Assembly> > Adicao::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Carregar operandos (op[0], op[1])
	loadReg(code,op[0],TM::t0);
	loadReg(code,op[1],TM::t1);
	// Realiza adicao, no temp t2
	code.emplace_back(new TM::ADD(TM::t2,TM::t0,TM::t1));	// ADD t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2);
	
	return code;
}	
Adicao::Adicao(Token _dst, Token _op1, Token _op2): Operacao("Adicao",_dst,_op1,_op2) {}

// Multiplicacao
list<shared_ptr<Assembly> > Multiplicacao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carregar operandos (op[0], op[1])
	loadReg(code,op[0],TM::t0);
	loadReg(code,op[1],TM::t1);
	// Realiza multiplicacao, no temp t2
	code.emplace_back(new TM::MUL(TM::t2,TM::t0,TM::t1));	// MUL t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2);
		
	return code;
}	

Multiplicacao::Multiplicacao(Token _dst, Token _op1, Token _op2): Operacao("Multiplicacao",_dst,_op1,_op2) {}

// Divisao
list<shared_ptr<Assembly> > Divisao::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Carregar operandos (op[0], op[1])
	loadReg(code,op[0],TM::t0);
	loadReg(code,op[1],TM::t1);
	// Realiza divisao, no temp t2
	code.emplace_back(new TM::DIV(TM::t2,TM::t0,TM::t1));	// DIV t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2);
	
	return code;
}	
Divisao::Divisao(Token _dst, Token _op1, Token _op2): Operacao("Divisao",_dst,_op1,_op2) {}

// Subtracao
list<shared_ptr<Assembly> > Subtracao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carregar operandos (op[0], op[1])
	loadReg(code,op[0],TM::t0);
	loadReg(code,op[1],TM::t1);
	// Realiza divisao, no temp t2
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2);
		
	return code;
}	

Subtracao::Subtracao(Token _dst, Token _op1, Token _op2): Operacao("Subtracao",_dst,_op1,_op2) {}

// Assign
Atribuicao::Atribuicao(Token _dst, Token _op): Operacao("Atribuicao",_dst,_op) {}

list<shared_ptr<Assembly> > Atribuicao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carregar operando op[0]
	loadReg(code,op[0],TM::t0);
	// Realiza atribuicao, no temp t2
	storeReg(code,dst,TM::t0);	// dst = op[0]
	
	return code;
}	

// Ponteiros=====================================

LoadFromRef::LoadFromRef(Token _dst, Token _pointer): Instrucao("RefLoad"), dst(_dst),pointer(_pointer) {}

std::list<std::shared_ptr<Assembly> > LoadFromRef::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carrega ponteiro
	loadReg(code,pointer,TM::t0);
	// Le do ponteiro
	code.emplace_back(new TM::LD(TM::t1,0,TM::t0));	// LD t1,0(t0)
	// Armazena resultado
	storeReg(code,dst,TM::t1);
	
	return code;
}	

StoreInRef::StoreInRef(Token _src, Token _pointer): Instrucao("RefStore"),src(_src),pointer(_pointer) {}

std::list<std::shared_ptr<Assembly> > StoreInRef::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carrega ponteiro
	loadReg(code,pointer,TM::t0);
	// Carrega operando src
	loadReg(code,src,TM::t1);
	// Armazena no ponteiro
	code.emplace_back(new TM::ST(TM::t1,0,TM::t0));	// ST t1,0(t0)
	
	return code;
}


// Chamada de funcao============================
std::list<std::shared_ptr<Assembly> > BeginCall::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Aloca espaco para guardar o ra na pilha
	alocar(code,1,TM::t0,TM::sp);
	// Guarda ra na pilha
	code.emplace_back(new TM::ST(TM::ra,1,TM::sp)); // ST ra,1(sp)
		
	return code;
}
BeginCall::BeginCall(): Instrucao("BeginCall") {}


std::list<std::shared_ptr<Assembly> > Param::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Aloca espaco para guardar o argumento na pilha
	int space = 1;	// normalmente, deveria ser extraido de Token op
	alocar(code,space,TM::t0,TM::sp);
	// Guarda argumento na pilha
	storeReg(code,parametro,TM::sp);		
	return code;
}
Param::Param(Token _parametro): parametro(_parametro), Instrucao("Param") {}


list<shared_ptr<Assembly> > Call::gera_codigo() {
	list<shared_ptr<Assembly> > code;	
	// Configurar ra
	code.emplace_back(new TM::LDA(TM::ra,3,TM::pc));	// LA ra,3(pc)
	// TODO: Configura destino do salto
	//assert(ts[funcao].has("Label")) 
	//long int offset_jump = ((Label*)ts[op]["Label"])->offset();
	//code.emplace_back(new TM::LD(reg,offset_jump,TM::gp));				// ST reg,offset_jump(sp)
	// Realiza salto
	code.emplace_back(new TM::JEQ(TM::zero,0,TM::t0));	// JEQ zero,0(t0)
	
	
	return code;
}
Call::Call(Token _funcao): funcao(_funcao), Instrucao("Call") {}


// Saltos ======================================

list<shared_ptr<Assembly> > Goto::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	/*
	
	// Carregar constante
	code.emplace_back(new TM::LDC(TM::t0,0,TM::t0));		// LDC t0,0(t0)	
	code.emplace_back(new TM::JEQ(TM::t0,distancia,TM::pc));	// JEQ t0,distancia(pc)
	*/
	return code;
}
Goto::Goto(Token _label): Code::Goto(_label), Instrucao("Goto") {}
	
	
SaltoCondicional::SaltoCondicional(string _classe, Token _label): Code::Goto(_label), Instrucao(_classe) {}


list<shared_ptr<Assembly> > Beq::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	/*
	
	// Carregar operandos (op1, op2)
	loadReg(code,op1,TM::t0);
	loadReg(code,op2,TM::t1);
	// Subtrair
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1
	code.emplace_back(new TM::JEQ(TM::t2,distancia,TM::pc));	// JEQ t2,distancia(pc)
	*/
	
	return code;
}
Beq::Beq(Token _op1, Token _op2, Token _label): SaltoCondicional("Beq",_label), op1(_op1), op2(_op2) {}





