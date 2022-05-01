#include "Semantico.h"
#include "../Sintatico/Sintatico.h"

using namespace std;

Semantico::Semantico(Sintatico &sintatico) {

}

Bloco::Bloco(): std::vector<Token>() {}

Bloco::Bloco(vector<Token>& tokens): std::vector<Token>(tokens) {} 

bool Bloco::eval(Semantico& state) {return true;}


