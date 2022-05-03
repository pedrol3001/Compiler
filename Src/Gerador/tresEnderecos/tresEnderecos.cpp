#include "tresEnderecos.h"

#include "../LourdenTM/LourdenTM.h"

#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <cassert>


using namespace Addr3;
using namespace std;

Addr3::Instrucao::Instrucao(string _classe): classe(_classe) {}
Addr3::Instrucao::~Instrucao() {}
// Funcoes auxiliares ===========================


	
void loadTemp(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg) {
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

void storeTemp(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg) {
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

// Input/output =================================
	
list<shared_ptr<Assembly> > Read::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Le input
	code.emplace_back(new TM::IN(TM::r0,TM::r0,TM::r0));		// IN r0,r0,r0	
	// Armazena input	
	storeTemp(code,op,TM::r0);
		
	return code;
}	
Read::Read(Token _op): op(_op), Instrucao("Read") {}

list<shared_ptr<Assembly> > Print::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	
	// Carrega operando	
	loadTemp(code,op,TM::r0);
	// Escreve output
	code.emplace_back(new TM::OUT(TM::r0,TM::r0,TM::r0));		// OUT r0,r0,r0	
		
	return code;
}	
Print::Print(Token _op): op(_op), Instrucao("Print") {}

// Declaracao ==================================
list<shared_ptr<Assembly> > Global::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Carregar constante
	code.emplace_back(new TM::LDC(TM::r0,1,TM::r0));		//  LDC r0,-1(r0)	
	// Aloca espaco na pilha
	code.emplace_back(new TM::SUB(TM::gp,TM::gp,TM::r0));	//  SUB gp,gp,r0	
	
	return code;
}	
Global::Global(Token _op): op(_op), Instrucao("Global") {}

list<shared_ptr<Assembly> > Aloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Carregar constante
	code.emplace_back(new TM::LDC(TM::r0,1,TM::r0));		// LDC r0,1(r0)	
	// Aloca espaco na pilha
	code.emplace_back(new TM::SUB(TM::sp,TM::sp,TM::r0));	// SUB sp,sp,r0	sp--
		
	return code;
}	
Aloca::Aloca(Token _op): op(_op), Instrucao("Aloca") {}

list<shared_ptr<Assembly> > Desaloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Carregar constante
	code.emplace_back(new TM::LDC(TM::r0,1,TM::r0));		// LDC r0,1(r0)	
	// Desaloca espaco na pilha
	code.emplace_back(new TM::ADD(TM::sp,TM::sp,TM::r0));	// SUB sp,sp,r0	sp++
			
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
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza adicao, no temp r2
	code.emplace_back(new TM::ADD(TM::r2,TM::r0,TM::r1));	// ADD r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
	
	return code;
}	
Adicao::Adicao(Token _dst, Token _op1, Token _op2): Operacao("Adicao",_dst,_op1,_op2) {}

// Multiplicacao
list<shared_ptr<Assembly> > Multiplicacao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza multiplicacao, no temp r2
	code.emplace_back(new TM::MUL(TM::r2,TM::r0,TM::r1));	// MUL r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
		
	return code;
}	

Multiplicacao::Multiplicacao(Token _dst, Token _op1, Token _op2): Operacao("Multiplicacao",_dst,_op1,_op2) {}

// Divisao
list<shared_ptr<Assembly> > Divisao::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza divisao, no temp r2
	code.emplace_back(new TM::DIV(TM::r2,TM::r0,TM::r1));	// DIV r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
	
	return code;
}	
Divisao::Divisao(Token _dst, Token _op1, Token _op2): Operacao("Divisao",_dst,_op1,_op2) {}

// Subtracao
list<shared_ptr<Assembly> > Subtracao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carregar operandos (op[0], op[1])
	loadTemp(code,op[0],TM::r0);
	loadTemp(code,op[1],TM::r1);
	// Realiza divisao, no temp r2
	code.emplace_back(new TM::SUB(TM::r2,TM::r0,TM::r1));	// SUB r2,r0,r1
	// Armazena valor obtido em r2 no destino
	storeTemp(code,dst,TM::r2);
		
	return code;
}	

Subtracao::Subtracao(Token _dst, Token _op1, Token _op2): Operacao("Subtracao",_dst,_op1,_op2) {}

// Assign
Atribuicao::Atribuicao(Token _dst, Token _op): Operacao("Atribuicao",_dst,_op) {}

list<shared_ptr<Assembly> > Atribuicao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	// Carregar operando op[0]
	loadTemp(code,op[0],TM::r0);
	// Realiza atribuicao, no temp r2
	storeTemp(code,dst,TM::r0);	// dst = op[0]
	
	return code;
}	


// Saltos ======================================
Label::Label(Token _token): token(_token), Instrucao("Label") {}

list<shared_ptr<Assembly> > Label::gera_codigo() {
	return list<shared_ptr<Assembly> >();	// Faz nada
}	

list<shared_ptr<Assembly> > Salto::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	/*
	
	// Carregar constante
	code.emplace_back(new TM::LDC(TM::r0,0,TM::r0));		// LDC r0,0(r0)	
	code.emplace_back(new TM::JEQ(TM::r0,distancia,TM::pc));	// JEQ r0,distancia(pc)
	*/
	return code;
}
Salto::Salto(Token _label): label(_label), Instrucao("Salto") {}
	
	
SaltoCondicional::SaltoCondicional(string _classe, Token _label): label(_label), Instrucao(_classe) {}

list<shared_ptr<Assembly> > Beq::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	/*
	
	// Carregar operandos (op1, op2)
	loadTemp(code,op1,TM::r0);
	loadTemp(code,op2,TM::r1);
	// Subtrair
	code.emplace_back(new TM::SUB(TM::r2,TM::r0,TM::r1));	// SUB r2,r0,r1
	code.emplace_back(new TM::JEQ(TM::r2,distancia,TM::pc));	// JEQ r2,distancia(pc)
	*/
	
	return code;
}
Beq::Beq(Token _op1, Token _op2, Token _label): SaltoCondicional("Beq",_label), op1(_op1), op2(_op2) {}





