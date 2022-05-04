#ifndef LourdenTM_h
#define LourdenTM_h

#include "../Codigo/Codigo.h"
#include "../Assembly/Assembly.h"
#include "../../Token/Token.h"

#include <string>

namespace TM {

	enum Reg {
		t0,t1,t2,zero,gp,ra,sp,	// zero = 0, gp = global pointer, ra = return adress, sp = stack pointer
		pc	// nao use
	};
	
	struct Instrucao: public Assembly {
		virtual std::string str();	
		Instrucao(std::string _nome, bool _is_label);
		virtual ~Instrucao();
		protected:
			virtual std::string InstStr();
		private:
			std::string nome;
	};
	
	struct Label: public Instrucao, public Code::Label {	// opcode r1,r2,r3
		Label(Token _label);
		std::string str();	
	};

	struct tipoRO: public Instrucao {	// opcode r1,r2,r3
		Reg r1,r2,r3;
		tipoRO(std::string _nome, Reg _r1, Reg _r2, Reg _r3);
		virtual ~tipoRO();
		std::string InstStr();	
	};
	#define INST_RO(tipo,nome) \
	struct tipo: public tipoRO { \
		tipo (Reg _r1, Reg _r2, Reg _r3): tipoRO(nome,_r1,_r2,_r3) {}\
	};
	
	// IO
	INST_RO(IN,"IN")	// stdin  >> r1 (ignora r2 e r3)
	INST_RO(OUT,"OUT")	// stdout << r1 (ignora r2 e r3) 
	// Aritmeticas
	INST_RO(ADD,"ADD")	// r1 = r2 + r3
	INST_RO(SUB,"SUB")	// r1 = r2 - r3
	INST_RO(MUL,"MUL")	// r1 = r2 * r3
	INST_RO(DIV,"DIV")	// r1 = r2 / r3
	// Fim de programa
	INST_RO(HALT,"HALT")	// Termina o programa (ignora r1,r2 e r3)
	
	struct tipoRM: public Instrucao {	//  opcode r1,offset(r2)
		Reg r1; int offset; Reg r2;
		tipoRM(std::string _nome, Reg _r1, int _offset, Reg _r2);
		virtual ~tipoRM();
		std::string InstStr();		
	};
	#define INST_RM(tipo,nome) \
	struct tipo: public tipoRM { \
		tipo (Reg _r1, int _offset, Reg _r2): tipoRM(nome,_r1,_offset,_r2) {}\
	};
	// Load
	INST_RM(LDC,"LDC")	// r1 = offset (ignora r2)
	INST_RM(LDA,"LDA")	// r1 = address (r2+offset)
	INST_RM(LD,"LD")	// r1 = carrega word endereçada por address (r2+offset)
	// Store
	INST_RM(ST,"ST")	// guarda r1 no endereço address (r2+offset)
	// Jump
	INST_RM(JEQ,"JEQ")	// r1==0? jump r2+offset
	INST_RM(JNE,"JNE")	// r1!=0? jump r2+offset
	INST_RM(JLT,"JLT")	// r1< 0? jump r2+offset
	INST_RM(JLE,"JLE")	// r1<=0? jump r2+offset
	INST_RM(JGT,"JGT")	// r1> 0? jump r2+offset
	INST_RM(JGE,"JGE")	// r1>=0? jump r2+offset
	
	struct tipoRM_Relativo: public Instrucao, public Code::Goto {	//  opcode r1,offset(r2)
		Reg r1;
		tipoRM_Relativo(std::string _nome, Reg _r1, Token _label);
		std::string InstStr();	
		virtual ~tipoRM_Relativo();	
	};
	#define INST_RM_REL(tipo,nome) \
	struct tipo: public tipoRM_Relativo { \
		tipo (Reg _r1, Token _label): tipoRM_Relativo(nome,_r1,_label) {}\
	};
	INST_RM_REL(RJEQ,"JEQ")	// r1==0? jump pc+label_distance
	INST_RM_REL(RJNE,"JNE")	// r1!=0? jump pc+label_distance
	INST_RM_REL(RJLT,"JLT")	// r1< 0? jump pc+label_distance
	INST_RM_REL(RJLE,"JLE")	// r1<=0? jump pc+label_distance
	INST_RM_REL(RJGT,"JGT")	// r1> 0? jump pc+label_distance
	INST_RM_REL(RJGE,"JGE")	// r1>=0? jump pc+label_distance
}


#endif
