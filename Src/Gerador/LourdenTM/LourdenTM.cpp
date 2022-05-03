#include "LourdenTM.h"

#include <sstream>

using namespace std;
using namespace TM;


Inst::Inst(string _nome): Assembly(_nome) {}

string Inst::str() {
	stringstream ss;
	ss << nome << ' ' << InstStr();
	return ss.str();
}
Inst::~Inst() {}


// Tipo RO
tipoRO::tipoRO(string _nome, Reg _r1, Reg _r2, Reg _r3): Inst(_nome), r1(_r1), r2(_r2), r3(_r3) {}
std::string tipoRO::InstStr() {
	stringstream ss;
	ss << r1 << ',' << r2 << ',' << r3;	// opcode r1,r2,r3
	return ss.str();
}
tipoRO::~tipoRO() {}

// Tipo RM
tipoRM::tipoRM(string _nome, Reg _r1, int _offset, Reg _r2): Inst(_nome), r1(_r1), offset(_offset), r2(_r2) {}
std::string tipoRM::InstStr() {
	stringstream ss;
	ss << r1 << ',' << offset << '(' << r2 << ')';	// opcode r1,offset(r2)
	return ss.str();
}
tipoRM::~tipoRM() {}






