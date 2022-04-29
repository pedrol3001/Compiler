#include "assemblyRiscV.h"

#include <sstream>

using namespace std;
using namespace RV;

static vector<string> regStr = {
	"zero",
	"ra","sp","gp","tp",
	"t0","t1","t2","t3","t4","t5","t6",
	"s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
	"a0","a1","a2","a3","a4","a5","a6","a7",
	"pc"
};

// Tipo R
tipoR::tipoR(string _nome): Assembly(_nome) {
	rd=rs1=rs2=zero;
}
std::string tipoR::toStr() {
	stringstream ss;
	ss << nome << ' ' << regStr[rd] << ',' << regStr[rs1] << ',' << regStr[rs2];	// nome rd,rs1,rs2  
	return ss.str();
}

// Tipo I
tipoI::tipoI(string _nome): Assembly(_nome) {
	rd=rs1=zero; imm=0;
}
std::string tipoI::toStr() {
	stringstream ss;
	ss << nome << ' ' << regStr[rd] << ',' << regStr[rs1] << ',' << imm;	// nome rd,rs1,imm  
	return ss.str();
}

// Tipo S
tipoS::tipoS(string _nome): Assembly(_nome) {
	r1=zero; imm=0; r2=zero;
}
std::string tipoS::toStr() {
	stringstream ss;
	ss << nome << ' ' << regStr[r1] << ',' << imm << '(' << regStr[r2] << ')';	// nome r1,imm(r2)  
	return ss.str();
}


// Tipo B
tipoB::tipoB(string _nome): Assembly(_nome) {
	rs1=rs2=zero; label=0;
}
std::string tipoB::toStr() {
	stringstream ss;
	ss << nome << ' ' << regStr[rs1] << ',' << regStr[rs2] << ',' << label;	// nome rd,rs1,label  
	return ss.str();
}


// Tipo J
tipoJ::tipoJ(string _nome): Assembly(_nome) {
	rs1=zero; label=0;
}
std::string tipoJ::toStr() {
	stringstream ss;
	ss << nome << ' ' << regStr[rs1] << ','  << label;	// nome rs1,label  
	return ss.str();
}


// Tipo U
tipoU::tipoU(string _nome): Assembly(_nome) {
	rd=zero; imm=0;
}
std::string tipoU::toStr() {
	stringstream ss;
	ss << nome << ' ' << regStr[rd] << ','  << imm;	// nome rd,imm  
	return ss.str();
}






