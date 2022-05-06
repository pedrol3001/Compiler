#include "Codigo.h"
 
#include <cassert>
 
using namespace Code;
using namespace std;

Codigo::Codigo(bool _ignore): ignore_(_ignore) {}
bool Codigo::ignore() {return ignore_;}

void Codigo::update(long long int _linha) {linha=_linha;}	// Atualiza linha
long long int Codigo::getLinha() {
	assert(linha>=0);
	return linha;	// retorna linha
}	
Codigo::~Codigo() {}	

Goto::Goto(Token _label): label(_label) {}
long long int Goto::readRef() {
	TabSim& ts = TabSim::getInstance();
	assert(ts[label].has("LabelVal"));
	return ((LabelVal*)ts[label]["LabelVal"])->getLinha();
}

	
Label::Label(Token _label): label(_label) {}
void Label::setRef(long long int linha) {
	TabSim& ts = TabSim::getInstance();
	assert(ts[label].has("LabelVal"));
	((LabelVal*)ts[label]["LabelVal"])->setLinha(linha);
}
