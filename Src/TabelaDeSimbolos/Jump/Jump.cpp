#include "Jump.h"

#include <cassert>

void LabelVal::setLinha(long long int _linha) {
	linha=_linha;
}
long long int LabelVal::getLinha() {
	assert(linha>=0);
	return linha;
}
LabelVal::LabelVal(): Atributo("LabelVal") {}
