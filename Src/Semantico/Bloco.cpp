
#include <vector>
#include "Bloco.h"
#include "../Token/Token.h"

using namespace std;

/**********************************************/
Bloco::~Bloco() {}

Bloco::Bloco() {}

Bloco::Bloco(const vector<Token>& _tokens): tokens(_tokens) {} 

bool Bloco::analisar(Semantico& state) {return true;}
void Bloco::gerar(Semantico& state) {}
int Bloco::eval(Semantico& state) {}
// Debug
std::vector<Token> Bloco::getTokens() {return tokens;}


// |a|, |b|, |obj.x|, |*c|, |&d|, |v[5+7]|, |a * f(b) + obj.x|, |f(g(h(2)))|
Expressao::Expressao(const vector<Token> &notacaoPolonesaReversa): Bloco(notacaoPolonesaReversa) { }

Expressao::eval(Semantico& state) {
    stack<int> operandos;
    for(Token t : tokens) {
        
    }
}

//bool Expressao::analisar(Semantico& state) {return true;}
void Expressao::gerar(Semantico& state) {}


Nada::Nada(): Bloco() {}


enum tipo { _int, _float, _void };

struct Escopo: Bloco{
    vector<Bloco> blocos;
    Semantico state;
    Escopo(vector<Bloco> _blocos, Semantico& _state): blocos(_blocos), state(_state) {}
        for(Bloco* bloco : blocos) {
            if(bloco->eval(state)){

            }
        }
    }
}

/* não serve pra nada?
struct Tipo: Bloco {
    tipo t;
    Tipo(tipo _t) : t(_t), exp(_exp) { }
    int eval(Semantico& state) {return true;}
};*/

struct Declaracao: Bloco{
    vector<Token> vars;
    tipo t;
    Declaracao() { }
    Declaracao(tipo _t, vector<Token> &_vars) : t(_t), vars(_vars) { }
    int eval(Semantico& state) {
        bool status = true;
        for(Token var : vars)
            status = status && state.declarar(var, t);
        return status;
    }
};

struct Funcao: Bloco {
    Token var;
    tipo t;
    vector<Token> args;
    vector<tipo> tipos;
    Funcao(Token _var, tipo _t, vector<Token> &_args, vector<tipo> &_tipos) : var(_var), t(_t), args(_args), tipos(_tipos) { }
    int eval(Semantico& state) {
        bool status = state.funcao(var, t);
        for(tipo t : tipos)
            status = status && t != _void;

        set<string> names;
        for(Token arg : args){
            if names.find(arg.obter_nome()) != names.end())
                status = false;
            names.insert(arg.obter_nome());
        }

        return status;
    }
};

struct Return: Bloco {
    Expressao expr;
    Return() { }
    Return(Expressao _expr) : expr(_expr) { }
    int eval(Semantico& state) {
        return expr.eval(state);
    }
};

struct If: Bloco {
    Expressao cond;
    If(Expressao _cond) : cond(_cond) { }
    int eval(Semantico& state) {
        return cond.eval(state);
    }
};

struct Else: Bloco { };

struct For: Bloco {
    Atribuicao decl;
    Expressao expr1;
    Expressao expr2;
    For(Atribuicao _decl, Expressao _expr1, Expressao _expr2) : decl(_decl), expr1(_expr1), expr2(_expr2) { }
    int eval(Semantico& state) {
        return decl.eval(state) && expr1.eval(state) && expr2.eval(state);
    }
};

struct While: Bloco {
    Expressao cond;
    While(Expressao _cond) : cond(_cond) { }
    int eval(Semantico& state) {
        return cond.eval(state);
    }
};