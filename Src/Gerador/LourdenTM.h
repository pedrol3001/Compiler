#ifndef LourdenTM_h
#define LourdenTM_h

#include "Gerador.h"

#include <string>

namespace TM {

	enum Reg {
		r0,r1,r2,r3,r4,r5,r6,
		pc	// nao use
	};
	
	struct Inst: public Assembly {
		std::string str();	
		Inst(std::string _nome, int _line);
		protected:
			virtual std::string InstStr()=0;
		private:
			int line;
	};

	struct tipoRO: public Inst {	// opcode r1,r2,r3
		Reg r1,r2,r3;
		tipoRO(std::string _nome, Reg _r1, Reg _r2, Reg _r3, int _line);
		std::string InstStr();	
	};
	#define INST_RO(tipo,nome) \
	struct tipo: public tipoRO { \
		tipo (Reg _r1, Reg _r2, Reg _r3, int _line): tipoRO(nome,_r1,_r2,_r3,_line) {}\
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
	
	struct tipoRM: public Inst {	//  opcode r1,offset(r2)
		Reg r1; int offset; Reg r2;
		tipoRM(std::string _nome, Reg _r1, int _offset, Reg _r2, int _line);
		std::string InstStr();		
	};
	#define INST_RM(tipo,nome) \
	struct tipo: public tipoRM { \
		tipo (Reg _r1, int _offset, Reg _r2, int _line): tipoRM(nome,_r1,_offset,_r2,_line) {}\
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
}


#endif