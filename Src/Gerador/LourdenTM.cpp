#include "LourdenTM.h"

#include <sstream>

using namespace std;
using namespace TM;


Inst::Inst(string _nome, int _line): Assembly(_nome), line(_line){}

string Inst::toStr() {
	stringstream ss;
	ss << line << ": " << InstToStr();
	return ss.str();
}


// Tipo RO
tipoRO::tipoRO(string _nome, Reg _r1, Reg _r2, Reg _r3, int _line): Inst(_nome,_line), r1(_r1), r2(_r2), r3(_r3) {}
std::string tipoRO::InstToStr() {
	stringstream ss;
	ss << nome << ' ' << r1 << ',' << r2 << ',' << r3;	// opcode r1,r2,r3
	return ss.str();
}

// Tipo RM
tipoRM::tipoRM(string _nome, Reg _r1, int _offset, Reg _r2, int _line): Inst(_nome, _line), r1(_r1), offset(_offset), r2(_r2) {}
std::string tipoRM::InstToStr() {
	stringstream ss;
	ss << nome << ' ' << r1 << ',' << offset << '(' << r2 << ')';	// opcode r1,offset(r2)
	return ss.str();
}






