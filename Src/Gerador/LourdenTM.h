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
		std::string toStr();	
		Inst(std::string _nome, int _line);
		protected:
			virtual std::string InstToStr()=0;
		private:
			int line;
	};

	struct tipoRO: public Inst {	//
		Reg r1,r2,r3;
		tipoRO(std::string _nome, Reg _r1, Reg _r2, Reg _r3, int _line);
		std::string InstToStr();	
	};
	#define INST_RO(name) struct name: public tipoRO { \
		name (Reg _r1, Reg _r2, Reg _r3, int _line): tipoRO("name",_r1,_r2,_r3,_line) {}\
	};
	
	// IO
	INST_RO(IN);	// stdin  >> r1 (ignora r2 e r3)
	INST_RO(OUT);	// stdout << r1 (ignora r2 e r3) 
	// Aritmeticas
	INST_RO(ADD);	// 
	INST_RO(SUB);
	INST_RO(MUL);
	INST_RO(DIV);
	// Fim de programa
	INST_RO(HALT);
	
	struct tipoRM: public Inst {
		Reg r1; int offset; Reg r2;
		tipoRM(std::string _nome, Reg _r1, int _offset, Reg _r2, int _line);
		std::string InstToStr();		
	};
	
	#define INST_RM(name) struct name: public tipoRM { \
		name (Reg _r1, int _offset, Reg _r2, int _line): tipoRM("name",_r1,_offset,_r2,_line) {}\
	};
	// address = (contents of r2) + offset
	// Load
	INST_RM(LDC)
	INST_RM(LDA)
	INST_RM(LD)
	// Store
	INST_RM(ST)	// store r1 in address
	// Jump
	INST_RM(JEQ)	// r1==0? jump r2+offset
	INST_RM(JNE)	// r1!=0? jump r2+offset
	INST_RM(JLT)	// r1< 0? jump r2+offset
	INST_RM(JLE)	// r1<=0? jump r2+offset
	INST_RM(JGT)	// r1> 0? jump r2+offset
	INST_RM(JGE)	// r1>=0? jump r2+offset
}


#endif
