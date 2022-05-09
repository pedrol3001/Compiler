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
	for(int i=0; i<ops.size(); i++) {
		Token token = ops[i];
		if(Addr3ts[token].has("VarLocal")) {
			Addr3ts[token].getAtt<VarLocal>("VarLocal")->setOffset(corretor.sp);
			offsets[i] = Addr3ts[token].getAtt<VarLocal>("VarLocal")->offset();
		}
		if(Addr3ts[token].has("VarGlobal")) {
			Addr3ts[token].getAtt<VarGlobal>("VarGlobal")->setOffset(corretor.gp);
			offsets[i] = Addr3ts[token].getAtt<VarGlobal>("VarGlobal")->offset();
		}
	}
}

Instrucao::Instrucao(std::string _classe, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops(), offsets() {}
	
Instrucao::Instrucao(std::string _classe, Token _op, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops(), offsets(1) {
	ops.push_back(_op);
}
Instrucao::Instrucao(std::string _classe, Token _op1, Token _op2, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops(), offsets(2) {
	ops.push_back(_op1);
	ops.push_back(_op2);
}
Instrucao::Instrucao(std::string _classe, Token _op1, Token _op2, Token _op3, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops(), offsets(3) {
	ops.push_back(_op1);
	ops.push_back(_op2);
	ops.push_back(_op3);
}
Instrucao::Instrucao(std::string _classe, vector<Token>& _ops, bool _ignore): 
	classe(_classe), Code::Codigo(_ignore), ops(_ops), offsets(_ops.size()){}
Instrucao::~Instrucao() {}

// Funcoes auxiliares ===========================
	
void loadReg(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg, long int offset) {	
	TabSim &ts = TabSim::getInstance();
	assert(ts[op].has("VarGlobal") || ts[op].has("VarLocal") || ts[op].has("IntVal") || ts[op].has("LabelVal"));
	
	int check_counter=0;
	
	if(ts[op].has("IntVal")) {
		long long int imm =  ((IntVal*)ts[op]["IntVal"])->val;
		code.emplace_back(new TM::LDC(reg,imm,TM::zero));	// loadl: LDC reg,val(zero) 
		check_counter++;
	}
	if(ts[op].has("VarGlobal")) {
		code.emplace_back(new TM::LD(reg,offset,TM::gp));	// loadl: LD reg,-offset(gp)
		check_counter++;
	}
	if(ts[op].has("VarLocal")) {
		code.emplace_back(new TM::LD(reg,offset,TM::sp));	// loadl: LD reg,-offset(sp)
		check_counter++;
	}
	if(ts[op].has("LabelVal")) {
		code.emplace_back(new TM::LoadLabel(reg,op));		// loadl: LD reg,LABEL(zero)
		check_counter++;
	}
	assert(check_counter==1);
}

void storeReg(list<shared_ptr<Assembly> >& code, Token op,TM::Reg reg, long int offset) {
	TabSim &ts = TabSim::getInstance();
	
	assert(ts[op].has("VarGlobal") || ts[op].has("VarLocal"));
	assert(!(ts[op].has("VarGlobal")&& ts[op].has("VarLocal")));
		
	if(ts[op].has("VarGlobal")) {
		code.emplace_back(new TM::ST(reg,offset,TM::gp));				// storel: ST reg,-offset(gp)
	}
	if(ts[op].has("VarLocal")) {
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
	code.emplace_back(new TM::LDA(TM::sp,0,TM::gp)); // LDA sp,0(gp)    # Pilha começa após variáveis globais

	return code;
}
SetLocal::SetLocal(): Instrucao("SetLocal") {}


std::list<std::shared_ptr<Assembly> > SetArray::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("SetArray"));
	code.emplace_back(new TM::LDA(TM::t0,2,TM::sp)); 	// LDA t0,2(sp)    # Configurar ponteiro de array
	code.emplace_back(new TM::ST(TM::t0,1,TM::sp)); 	// ST t0,1(sp)    # Salvar ponteiro de array
	return code;
}
SetArray::SetArray(): Instrucao("SetArray") {}

// Input/output =================================
	
list<shared_ptr<Assembly> > Read::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Read"));
	// Le input
	code.emplace_back(new TM::IN(TM::t0,TM::t0,TM::t0));		// IN t0,t0,t0	
	// Armazena input	
	storeReg(code,op,TM::t0,offsets[0]);
		
	return code;
}	
Read::Read(Token _op): op(_op), Instrucao("Read",_op) {}

list<shared_ptr<Assembly> > Print::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Print " + TabSim::getInstance()[op].getAtt<StrAtt>("StrAtt")->str));
	
	// Carrega operando	
	loadReg(code,op,TM::t0,offsets[0]);
	// Escreve output
	code.emplace_back(new TM::OUT(TM::t0,TM::t0,TM::t0));		// OUT t0,t0,t0	
		
	return code;
}	
Print::Print(Token _op): op(_op), Instrucao("Print",_op) {}

// Declaracao ==================================

list<shared_ptr<Assembly> > AlocaGlobal::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("AlocaGlobal"));
	assert(Addr3ts[op].has("VarGlobal"));
	int space = ((VarGlobal*)Addr3ts[op]["VarGlobal"])->size;
	alocar(code,space,TM::t0,TM::gp);		
	return code;
}	

AlocaGlobal::AlocaGlobal(Token _op): op(_op), Instrucao("AlocaGlobal",_op) {}
void AlocaGlobal::acao(Corretor& corretor) {
	assert(Addr3ts[op].has("VarGlobal"));
	// Incrementar gp
	corretor.gp += Addr3ts[op].getAtt<VarGlobal>("VarGlobal")->size;
	// Configurar distancia da variavel
	Addr3ts[op].getAtt<VarGlobal>("VarGlobal")->setDist(corretor.gp-1);
}

list<shared_ptr<Assembly> > Aloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Aloca"));
	assert(Addr3ts[op].has("VarLocal"));
	int space = ((VarLocal*)Addr3ts[op]["VarLocal"])->size;
	alocar(code,space,TM::t0,TM::sp);		
	return code;
}	
Aloca::Aloca(Token _op): op(_op), Instrucao("Aloca",_op) {}

list<shared_ptr<Assembly> > Desaloca::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Desaloca"));
	assert(Addr3ts[op].has("VarLocal"));
	int space = ((VarLocal*)Addr3ts[op]["VarLocal"])->size;
	desalocar(code,space,TM::t0,TM::sp);		
	return code;
}	
Desaloca::Desaloca(Token _op): op(_op), Instrucao("Desaloca",_op) {}
	
list<shared_ptr<Assembly> > Declarar::gera_codigo() {
	list<shared_ptr<Assembly> >  code;	
	code.emplace_back(new TM::Comentario("Declarar " + Addr3ts[op].getAtt<StrAtt>("StrAtt")->str));
	return code;
}
void Declarar::acao(Corretor& corretor) {
	assert(Addr3ts[op].has("VarLocal"));
	// Incrementar sp
	corretor.sp += Addr3ts[op].getAtt<VarLocal>("VarLocal")->size;
	// Configurar distancia da variavel
	Addr3ts[op].getAtt<VarLocal>("VarLocal")->setDist(corretor.sp-1);
}
Declarar::Declarar(Token _op): op(_op), Instrucao("Declarar",_op) {}
			
list<shared_ptr<Assembly> > Retirar::gera_codigo() {
	list<shared_ptr<Assembly> >  code;	
	code.emplace_back(new TM::Comentario("Retirar " + Addr3ts[op].getAtt<StrAtt>("StrAtt")->str));
	return code;
}
void Retirar::acao(Corretor& corretor) {
	assert(Addr3ts[op].has("VarLocal"));
	// Decrementar sp
	corretor.sp -= Addr3ts[op].getAtt<VarLocal>("VarLocal")->size;
}
Retirar::Retirar(Token _op): op(_op), Instrucao("Retirar",_op) {}

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


// Booleanas
std::list<std::shared_ptr<Assembly> > Less::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Less"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1	// t2 = t0-t1 (t0?t1 -> t0-t1?0)
	code.emplace_back(new TM::LDC(TM::t0,1,TM::zero));	// t0 = 1 = true
	code.emplace_back(new TM::JLT(TM::t2,1,TM::pc));	// if t2<0 goto (pc+1)+1
	code.emplace_back(new TM::LDC(TM::t0,0,TM::zero));	// t0 = 0 = false
	// Armazena valor obtido em t0 no destino
	storeReg(code,dst,TM::t0,offsets[0]);
	
	return code;
}
Less::Less(Token _dst, Token _op1, Token _op2): Operacao("Less",_dst,_op1,_op2) {}


std::list<std::shared_ptr<Assembly> > LessEqual::gera_codigo(){
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("LessEqual"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1	// t2 = t0-t1 (t0?t1 -> t0-t1?0)
	code.emplace_back(new TM::LDC(TM::t0,1,TM::zero));	// t0 = 1 = true
	code.emplace_back(new TM::JLE(TM::t2,1,TM::pc));	// if t2<=0 goto (pc+1)+1
	code.emplace_back(new TM::LDC(TM::t0,0,TM::zero));	// t0 = 0 = false
	// Armazena valor obtido em t0 no destino
	storeReg(code,dst,TM::t0,offsets[0]);
	
	return code;
}
LessEqual::LessEqual(Token _dst, Token _op1, Token _op2): Operacao("LessEqual",_dst,_op1,_op2) {}


std::list<std::shared_ptr<Assembly> > Equal::gera_codigo(){
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Equal"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1	// t2 = t0-t1 (t0?t1 -> t0-t1?0)
	code.emplace_back(new TM::LDC(TM::t0,1,TM::zero));	// t0 = 1 = true
	code.emplace_back(new TM::JEQ(TM::t2,1,TM::pc));	// if t2==0 goto (pc+1)+1
	code.emplace_back(new TM::LDC(TM::t0,0,TM::zero));	// t0 = 0 = false
	// Armazena valor obtido em t0 no destino
	storeReg(code,dst,TM::t0,offsets[0]);
	
	return code;
}
Equal::Equal(Token _dst, Token _op1, Token _op2): Operacao("Equal",_dst,_op1,_op2) {}	
	
std::list<std::shared_ptr<Assembly> > NotEqual::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("NotEqual"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1	// t2 = t0-t1 (t0?t1 -> t0-t1?0)
	code.emplace_back(new TM::LDC(TM::t0,1,TM::zero));	// t0 = 1 = true
	code.emplace_back(new TM::JNE(TM::t2,1,TM::pc));	// if t2!=0 goto (pc+1)+1
	code.emplace_back(new TM::LDC(TM::t0,0,TM::zero));	// t0 = 0 = false
	// Armazena valor obtido em t0 no destino
	storeReg(code,dst,TM::t0,offsets[0]);
	
	return code;
}
NotEqual::NotEqual(Token _dst, Token _op1, Token _op2): Operacao("NotEqual",_dst,_op1,_op2) {}

// Aritmeticas

// Adicao	
list<shared_ptr<Assembly> > Adicao::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Adicao"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	// Realiza adicao, no temp t2
	code.emplace_back(new TM::ADD(TM::t2,TM::t0,TM::t1));	// ADD t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2,offsets[0]);
	
	return code;
}	
Adicao::Adicao(Token _dst, Token _op1, Token _op2): Operacao("Adicao",_dst,_op1,_op2) {}

// Multiplicacao
list<shared_ptr<Assembly> > Multiplicacao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("Multiplicacao"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	// Realiza multiplicacao, no temp t2
	code.emplace_back(new TM::MUL(TM::t2,TM::t0,TM::t1));	// MUL t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2,offsets[0]);
		
	return code;
}	

Multiplicacao::Multiplicacao(Token _dst, Token _op1, Token _op2): Operacao("Multiplicacao",_dst,_op1,_op2) {}

// Divisao
list<shared_ptr<Assembly> > Divisao::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Divisao"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	// Realiza divisao, no temp t2
	code.emplace_back(new TM::DIV(TM::t2,TM::t0,TM::t1));	// DIV t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2,offsets[0]);
	
	return code;
}	
Divisao::Divisao(Token _dst, Token _op1, Token _op2): Operacao("Divisao",_dst,_op1,_op2) {}

// Subtracao
list<shared_ptr<Assembly> > Subtracao::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("Subtracao"));
	// Carregar operandos (op[0], op[1])
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	// Realiza divisao, no temp t2
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1
	// Armazena valor obtido em t2 no destino
	storeReg(code,dst,TM::t2,offsets[0]);
		
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
	loadReg(code,ops[1],TM::t0,offsets[1]);
	// Realiza atribuicao, no temp t2
	storeReg(code,dst,TM::t0,offsets[0]);
	if(op.size()==2) {
		// Copia o mesmo valor para ret
		storeReg(code,ops[2],TM::t0,offsets[2]);	// ret = op[1]
	}
	
	return code;
}	

// Ponteiros=====================================

LoadFromRef::LoadFromRef(Token _dst, Token _pointer): Instrucao("RefLoad",_dst,_pointer), dst(_dst),pointer(_pointer) {}

std::list<std::shared_ptr<Assembly> > LoadFromRef::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("LoadFromRef"));
	// Carrega ponteiro
	loadReg(code,pointer,TM::t0,offsets[1]);
	// Le do ponteiro
	code.emplace_back(new TM::LD(TM::t1,0,TM::t0));	// LD t1,0(t0)
	// Armazena resultado
	storeReg(code,dst,TM::t1,offsets[0]);
	
	return code;
}	

StoreInRef::StoreInRef(Token _pointer, Token _src): Instrucao("RefStore",_pointer,_src),src(_src),pointer(_pointer) {}

std::list<std::shared_ptr<Assembly> > StoreInRef::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("StoreInRef"));
	// Carrega ponteiro
	loadReg(code,pointer,TM::t0,offsets[0]);
	// Carrega operando src
	loadReg(code,src,TM::t1,offsets[1]);
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
void BeginCall::acao(Corretor& corretor) {
	corretor.param=0;
}


std::list<std::shared_ptr<Assembly> > Param::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("Param"));
	// Carrega operando
	loadReg(code,parametro,TM::t0,offsets[0]);		
	// Guarda operando alem da pilha
	code.emplace_back(new TM::ST(TM::t0,param_offset,TM::sp));
	return code;
}
Param::Param(Token _parametro): parametro(_parametro), Instrucao("Param",_parametro) {}
void Param::acao(Corretor& corretor) {
	Instrucao::acao(corretor);
	param_offset = corretor.param--;
}


list<shared_ptr<Assembly> > Call::gera_codigo() {
	list<shared_ptr<Assembly> > code;	
	code.emplace_back(new TM::Comentario("Call"));
	// Configurar ra
	code.emplace_back(new TM::LDA(TM::ra,2,TM::pc));	// LA ra,2(pc) (pc+1) +2
	// Carrega endereco do salto
	loadReg(code,funcao,TM::t0,offsets[0]);				
	// Realiza salto
	code.emplace_back(new TM::JEQ(TM::zero,0,TM::t0));	// JEQ zero,0(t0)
	// Recupera ra
	code.emplace_back(new TM::LD(TM::ra,1,TM::sp));	// LD ra,1(sp)
	// Desaloca espaco do ra
	desalocar(code,1,TM::t0,TM::sp);
	return code;
}
Call::Call(Token _funcao): funcao(_funcao), Instrucao("Call",_funcao) {}
void Call::acao(Corretor& corretor) {
	Instrucao::acao(corretor);
	corretor.param=0;
}

list<shared_ptr<Assembly> > InitialCall::gera_codigo() {
	list<shared_ptr<Assembly> > code;	
	code.emplace_back(new TM::Comentario("InitialCall"));
	// Configurar ra
	code.emplace_back(new TM::LDA(TM::ra,2,TM::pc));	// LA ra,2(pc) (pc+1) +2
	// Carrega endereco do salto
	loadReg(code,funcao,TM::t0,offsets[0]);				
	// Realiza salto
	code.emplace_back(new TM::JEQ(TM::zero,0,TM::t0));	// JEQ zero,0(t0)
	return code;
}
InitialCall::InitialCall(Token _funcao): funcao(_funcao), Instrucao("InitialCall",_funcao) {}
void InitialCall::acao(Corretor& corretor) {
	Instrucao::acao(corretor);
	corretor.param=0;
}

	
list<shared_ptr<Assembly> > Return::gera_codigo() {
	list<shared_ptr<Assembly> > code;	
	code.emplace_back(new TM::Comentario("Return"));
	code.emplace_back(new TM::JEQ(TM::zero,0,TM::ra));	// JEQ 0,0(ra)
	return code;
}	
Return::Return(): Instrucao("Return") {}

// Saltos ======================================

list<shared_ptr<Assembly> > Goto::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario("Goto " + showToken(label)));	
	code.emplace_back(new TM::RJEQ(TM::zero,label));	// JEQ t0,distancia-1(pc)
	return code;
}
Goto::Goto(Token _label): Code::Goto(_label), Instrucao("Goto",_label) {}
	
	
SaltoCondicional::SaltoCondicional(string _classe, Token _op1, Token _op2, Token _label): Code::Goto(_label), Instrucao(_classe,_label,_op1,_op2) {}


list<shared_ptr<Assembly> > Beq::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	
	code.emplace_back(new TM::Comentario("Beq"));	
	// Carregar operandos (op1, op2)
	loadReg(code,ops[1],TM::t0,offsets[1]);
	loadReg(code,ops[2],TM::t1,offsets[2]);
	// Subtrair
	code.emplace_back(new TM::SUB(TM::t2,TM::t0,TM::t1));	// SUB t2,t0,t1
	// Realizar salto
	code.emplace_back(new TM::RJEQ(TM::t2,label));		// JEQ t2,distancia(pc)
		
	return code;
}
Beq::Beq(Token _op1, Token _op2, Token _label): 
	SaltoCondicional("Beq",_op1,_op2,_label) {}

// Outros =================================================================================================

std::list<std::shared_ptr<Assembly> > Comentario::gera_codigo() {
	list<shared_ptr<Assembly> >  code;
	code.emplace_back(new TM::Comentario(str));
	return code;
}
Comentario::Comentario(std::string _str): Instrucao("Comentario",true), str(_str) {}


std::list<std::shared_ptr<Assembly> > Exit::gera_codigo() {
	list<shared_ptr<Assembly> > code;
	code.emplace_back(new TM::Comentario("Exit"));	
	code.emplace_back(new TM::HALT(TM::zero,TM::zero,TM::zero));		// HALT 0,0,0
	return code;
}
Exit::Exit(): Instrucao("Exit") {}

