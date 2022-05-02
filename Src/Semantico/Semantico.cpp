#include "Semantico.h"
#include "Bloco.cpp"
#include "../Sintatico/Sintatico.h"

using namespace std;

Semantico::Semantico(Sintatico &sintatico) {
    bool status = true;
    Semantico state;
    Escopo global(sintatico.container, state);
}

