#include "LourdenTM.h"

#include <sstream>

using namespace std;
using namespace TM;


Instrucao::Instrucao(string _nome, bool _is_label): nome(_nome), Assembly(_is_label) {}

string Instrucao::str() {
	stringstream ss;
	ss << nome << ' ' << InstStr();
	return ss.str();
}
std::string Instrucao::InstStr() {return string();}
Instrucao::~Instrucao() {}

// Label
Label::Label(Token _label): Instrucao("Label",true),Code::Label(_label) {}
std::string Label::str() {return string();}	

// Tipo RO
tipoRO::tipoRO(string _nome, Reg _r1, Reg _r2, Reg _r3): Instrucao(_nome,false), r1(_r1), r2(_r2), r3(_r3) {}
std::string tipoRO::InstStr() {
	stringstream ss;
	ss << r1 << ',' << r2 << ',' << r3;	// opcode r1,r2,r3
	return ss.str();
}
tipoRO::~tipoRO() {}

// Tipo RM
tipoRM::tipoRM(string _nome, Reg _r1, int _offset, Reg _r2): Instrucao(_nome,false), r1(_r1), offset(_offset), r2(_r2) {}
std::string tipoRM::InstStr() {
	stringstream ss;
	ss << r1 << ',' << offset << '(' << r2 << ')';	// opcode r1,offset(r2)
	return ss.str();
}
tipoRM::~tipoRM() {}

tipoRM_Relativo::tipoRM_Relativo(std::string _nome, Reg _r1, Token _label): Instrucao(_nome,false), r1(_r1), Code::Goto(_label) {}
std::string tipoRM_Relativo::InstStr() {
	stringstream ss;
	long long int distance = readRef()-getLinha();		// Distancia ate a label
	ss << r1 << ',' << distance << '(' << TM::pc << ')';	// opcode r1,distance(pc)
	return ss.str();
}
tipoRM_Relativo::~tipoRM_Relativo() {}






