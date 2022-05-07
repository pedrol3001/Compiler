#include "tresEnderecos.h"

#include "../LourdenTM/LourdenTM.h"

#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <cassert>

using namespace Addr3;
using namespace std;


TabSim& Addr3ts =TabSim::getInstance();

string showToken(Token t) {
	return TabSim::getInstance()[t].getAtt<StrAtt>("StrAtt")->str;
}
void Instrucao::acao(Corretor& corretor) {
	TabSim& Addr3ts=TabSim::getInstance();
	for(Token token: ops) {
		if(Addr3ts[token].has("VarLocal"))
			Addr3ts[token].getAtt<VarLocal>("VarLocal")->setOffset(corretor.sp);
		if(Addr3ts[token].has("VarGlobal"))
			Addr3ts[token].getAtt<VarGlobal>("VarGlobal")->setOffset(corretor.gp);
	}
}

Instrucao::Instrucao(std::string _classe, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops() {}
	
Instrucao::Instrucao(std::string _classe, Token _op, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops() {
	ops.push_back(_op);
}
Instrucao::Instrucao(std::string _classe, Token _op1, Token _op2, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops() {
	ops.push_back(_op1);
	ops.push_back(_op2);
}
Instrucao::Instrucao(std::string _classe, Token _op1, Token _op2, Token _op3, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops() {
	ops.push_back(_op1);
	ops.push_back(_op2);
	ops.push_back(_op3);
}
Instrucao::Instrucao(std::string _classe, vector<Token>& _ops, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops(_ops){}
Instrucao::~Instrucao() {}

// Funcoes auxiliares ===========================
	
void loadReg(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg) {	
	TabSim &ts = TabSim::getInstance();
	assert(ts[op].has("VarGlobal") || ts[op].has("VarLocal") || ts[op].has("IntVal") || ts[op].has("LabelVal"));
	
	if(ts[op].has("IntVal")) {
		long long int imm =  ((IntVal*)ts[op]["IntVal"])->val;
		code.emplace_back(new TM::LDC(reg,imm,TM::zero));			// loadl: LDC reg,val(zero) 
	}
	if(ts[op].has("VarGlobal")) {
		long int offset = ((VarGlobal*)ts[op]["VarGlobal"])->offset();
		code.emplace_back(new TM::LD(reg,offset,TM::gp));			// loadl: LD reg,-offset(gp)
	}
	if(ts[op].has("VarLocal")) {
		long int offset = ((VarLocal*)ts[op]["VarLocal"])->offset();
		code.emplace_back(new TM::LD(reg,offset,TM::sp));			// loadl: LD reg,-offset(sp)
	}
	if(ts[op].has("LabelVal")) {
		code.emplace_back(new TM::LoadLabel(reg,op));		// loadl: LD reg,LABEL(zero)
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

// Inicializacao ================================

list<shared_ptr<Assembly> > SetZero::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("SetZero"));
	// Configura global para ser o ultimo endereco valido
	code.emplace_back(new TM::LDC(TM::zero,0,TM::zero)); // LDC zero,0(zero)    # zero = 0
	
	return code;
}	
SetZero::SetZero(): Instrucao("SetZero") {}

list<shared_ptr<Assembly> > SetGlobal::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("SetGlobal"));
	// Configura global para ser o ultimo endereco valido
	code.emplace_back(new TM::LD(TM::gp,0,TM::zero)); // LD gp,0(zero)        # gp = endereço de maior tamanho permitido
	
	return code;
}
SetGlobal::SetGlobal(): Instrucao("SetGlobal") {}

list<shared_ptr<Assembly> > SetLocal::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("SetLocal"));
	// Configura ponteiro de pilha para ser apos gp
	code.emplace_back(new TM::LDA(TM::sp,-1,TM::gp)); // LDA sp,-1(gp)    # Pilha começa após variáveis globais

	return code;
}
SetLocal::SetLocal(): Instrucao("SetLocal") {}

// Input/output =================================
	
list<shared_ptr<Assembly> > Read::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Read"));
	// Le input
	code.emplace_back(new TM::IN(TM::t0,TM::t0,TM::t0));		// IN t0,t0,t0	
	// Armazena input	
	storeReg(code,op,TM::t0);
		
	return code;
}	
Read::Read(Token _op): op(_op), Instrucao("Read",_op) {}

list<shared_ptr<Assembly> > Print::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Print " + TabSim::getInstance()[op].getAtt<IdVal>("IdVal")->val));
	
	// Carrega operando	
	loadReg(code,op,TM::t0);
	// Escreve output
	code.emplace_back(new TM::OUT(TM::t0,TM::t0,TM::t0));		// OUT t0,t0,t0	
		
	return code;
}	
Print::Print(Token _op): op(_op), Instrucao("Print",_op) {}

// Declaracao ==================================
list<shared_ptr<Assembly> > AlocaGlobal::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("AlocaGlobal"));
	int space = 1;	// normalmente, deveria ser extraido de Token op
	alocar(code,space,TM::t0,TM::gp);		
	return code;
}	
AlocaGlobal::AlocaGlobal(Token _op): op(_op), Instrucao("AlocaGlobal",_op) {}
void AlocaGlobal::acao(Corretor& corretor) {
	assert(Addr3ts[op].has("VarGlobal"));
	// Configurar distancia da variavel
	Addr3ts[op].getAtt<VarGlobal>("VarGlobal")->setDist(corretor.sp);
	// Incrementar gp
	corretor.gp += Addr3ts[op].getAtt<VarGlobal>("VarGlobal")->size;
}

list<shared_ptr<Assembly> > Aloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Aloca"));
	int space = 1;	// normalmente, deveria ser extraido de Token op
	alocar(code,space,TM::t0,TM::sp);		
	return code;
}	
Aloca::Aloca(Token _op): op(_op), Instrucao("Aloca",_op) {}
void Aloca::acao(Corretor& corretor) {
	assert(Addr3ts[op].has("VarLocal"));
	// Configurar distancia da variavel
	Addr3ts[op].getAtt<VarLocal>("VarLocal")->setDist(corretor.sp);
	// Incrementar sp
	corretor.sp += Addr3ts[op].getAtt<VarLocal>("VarLocal")->size;
}

list<shared_ptr<Assembly> > Desaloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Desaloca"));
	int space = 1;	// normalmente, deveria ser extraido de Token op
	desalocar(code,space,TM::t0,TM::sp);		
	return code;
}	
Desaloca::Desaloca(Token _op): op(_op), Instrucao("Desaloca",_op) {}
void Desaloca::acao(Corretor& corretor) {
	assert(Addr3ts[op].has("VarLocal"));
	// Decrementar sp
	corretor.sp -= Addr3ts[op].getAtt<VarLocal>("VarLocal")->size;
}

// Operacao ====================================
Operacao::~Operacao() {}
Operacao::Operacao(string _classe, Token _dst, vector<Token>& _op): dst(_dst), op(_op), Instrucao(_classe,_op) {}
Operacao::Operacao(string _classe, Token _dst, Token _op): dst(_dst), Instrucao(_classe,_dst,_op) {
	op.emplace_back(_op);
}	
Operacao::Operacao(string _classe, Token _dst, Token _op1, Token _op2): 
	dst(_dst), Instrucao(_classe,_dst,_op1,_op2) {
	op.emplace_back(_op1);
	op.emplace_back(_op2);
}		
	
// Adicao	
list<shared_ptr<Assembly> > Adicao::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Adicao"));
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
	code.emplace_back(new TM::Comentario("Multiplicacao"));
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
	code.emplace_back(new TM::Comentario("Divisao"));
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
	code.emplace_back(new TM::Comentario("Subtracao"));
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
Atribuicao::Atribuicao(Token _ret, Token _dst, Token _op): Operacao("Atribuicao",_dst,_op,_ret) {}

list<shared_ptr<Assembly> > Atribuicao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("Atribuicao"));
	// Carregar operando op[0]
	loadReg(code,op[0],TM::t0);
	// Realiza atribuicao, no temp t2
	storeReg(code,dst,TM::t0);	// dst = op[0]
	if(op.size()==2) {
		// Copia o mesmo valor para ret
		storeReg(code,op[1],TM::t0);	// ret = op[1]
	}
	
	return code;
}	

// Ponteiros=====================================

LoadFromRef::LoadFromRef(Token _dst, Token _pointer): Instrucao("RefLoad",_dst,_pointer), dst(_dst),pointer(_pointer) {}

std::list<std::shared_ptr<Assembly> > LoadFromRef::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("LoadFromRef"));
	// Carrega ponteiro
	loadReg(code,pointer,TM::t0);
	// Le do ponteiro
	code.emplace_back(new TM::LD(TM::t1,0,TM::t0));	// LD t1,0(t0)
	// Armazena resultado
	storeReg(code,dst,TM::t1);
	
	return code;
}	

StoreInRef::StoreInRef(Token _src, Token _pointer): Instrucao("RefStore",_src,_pointer),src(_src),pointer(_pointer) {}

std::list<std::shared_ptr<Assembly> > StoreInRef::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("StoreInRef"));
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
	code.emplace_back(new TM::Comentario("BeginCall"));
	// Aloca espaco para guardar o ra na pilha
	alocar(code,1,TM::t0,TM::sp);
	// Guarda ra na pilha
	code.emplace_back(new TM::ST(TM::ra,1,TM::sp)); // ST ra,1(sp)
		
	return code;
}
BeginCall::BeginCall(): Instrucao("BeginCall") {}


std::list<std::shared_ptr<Assembly> > Param::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("Param"));
	// Aloca espaco para guardar o argumento na pilha
	int space = 1;	// normalmente, deveria ser extraido de Token op
	alocar(code,space,TM::t0,TM::sp);
	// Guarda argumento na pilha
	storeReg(code,parametro,TM::sp);		
	return code;
}
Param::Param(Token _parametro): parametro(_parametro), Instrucao("Param",_parametro) {}


list<shared_ptr<Assembly> > Call::gera_codigo() {
		
	list<shared_ptr<Assembly> > code;	
	code.emplace_back(new TM::Comentario("Call"));
	/*
	// Configurar ra
	code.emplace_back(new TM::LDA(TM::ra,3,TM::pc));	// LA ra,3(pc)
	// TODO: Configura destino do salto
	//assert(ts[funcao].has("Label")) 
	//long int offset_jump = ((Label*)ts[op]["Label"])->offset();
	//code.emplace_back(new TM::LD(reg,offset_jump,TM::gp));				// ST reg,offset_jump(sp)
	// Realiza salto
	code.emplace_back(new TM::JEQ(TM::zero,0,TM::t0));	// JEQ zero,0(t0)
	*/
	
	return code;
}
Call::Call(Token _ret, Token _funcao): ret(_ret), funcao(_funcao), Instrucao("Call",_ret,_funcao) {}

	
list<shared_ptr<Assembly> > Return::gera_codigo() {
		
	list<shared_ptr<Assembly> > code;	
	code.emplace_back(new TM::Comentario("Return"));
	if(has_value) {
	
	}
	// retornar para ra, empilhando retorno

	return code;
}	
Return::Return(Token _ret): ret(_ret), Instrucao("Return",_ret) {has_value=true;}
Return::Return(): Instrucao("Return") {}

// Saltos ======================================

list<shared_ptr<Assembly> > Goto::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Goto " + showToken(label)));
	/*
	
	// Carregar constante
	code.emplace_back(new TM::LDC(TM::t0,0,TM::t0));		// LDC t0,0(t0)	
	code.emplace_back(new TM::JEQ(TM::t0,distancia,TM::pc));	// JEQ t0,distancia(pc)
	*/
	return code;
}
Goto::Goto(Token _label): Code::Goto(_label), Instrucao("Goto",_label) {}
	
	
SaltoCondicional::SaltoCondicional(string _classe, Token _label): Code::Goto(_label), Instrucao(_classe,_label) {}


list<shared_ptr<Assembly> > Beq::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	
	code.emplace_back(new TM::Comentario("Beq"));	
	// Carregar operandos (op1, op2)
	loadReg(code,op1,TM::t0);
	loadReg(code,op2,TM::t1);
	// Subtrair
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1
	// Realizar salto
	code.emplace_back(new TM::RJEQ(TM::t2,label));		// JEQ t2,distancia(pc)
	
	return code;
}
Beq::Beq(Token _op1, Token _op2, Token _label): 
	SaltoCondicional("Beq",_label), op1(_op1), op2(_op2) {}

// Outros =================================================================================================

std::list<std::shared_ptr<Assembly> > Comentario::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario(str));
	return code;
}
Comentario::Comentario(std::string _str): Instrucao("Comentario",true), str(_str) {}

