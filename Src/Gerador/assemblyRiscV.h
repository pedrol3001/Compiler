#ifndef assemblyRiscV_h
#define assemblyRiscV_h

#include "Assembly.h"


#include <string>

namespace RV {

	enum Reg {
		zero,
		ra,sp,gp,tp,
		t0,t1,t2,t3,t4,t5,t6,
		s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,
		a0,a1,a2,a3,a4,a5,a6,a7,
		pc	// nao use
	};

	struct tipoR: public Assembly {	//
		Reg rd,rs1,rs2;
		std::string toStr();	
		tipoR(std::string _nome);	
	};
	
	
	struct tipoI: public Assembly {
		Reg rd; Reg rs1; int imm;
		std::string toStr();	
		tipoI(std::string _nome);		
	};
	
	struct tipoS: public Assembly {
		Reg r1; int imm; Reg r2;
		std::string toStr();	
		tipoS(std::string _nome);		
	};
	
	struct tipoB: public Assembly {
		Reg rs1; Reg rs2; int label;
		std::string toStr();	
		tipoB(std::string _nome);		
	};

	struct tipoJ: public Assembly {
		Reg rs1; int label;
		std::string toStr();
		tipoJ(std::string _nome);			
	};
	
	struct tipoU: public Assembly {
		Reg rd; int imm;
		std::string toStr();
		tipoU(std::string _nome);			
	};
}


#endif
