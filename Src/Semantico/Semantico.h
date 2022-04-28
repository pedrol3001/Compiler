#ifndef Semantico_h
#define Semantico_h

#include <vector>
#include "../Token/Token.h"
#include "../Sintatico/Sintatico.h"

struct Semantico {
    Semantico(Sintatico &sintatico);

};

struct State;

enum tipo { _int, _float, _void };

struct Comando {
      Comando();
      virtual void Update(Semantico& state);
};

// |int|, |float|, |void|
struct Tipo: Comando {
    tipo t;
    Tipo(tipo _t) : t(_t) { }
};

// |a|, |b|, |obj.x|, |*c|, |&d|, |v[5+7]|, |a * f(b) + obj.x|, |f(g(h(2)))|
struct Expressao: Comando {
    std::vector<Token> tokens;
    Expressao() { }
    Expressao(std::vector<Token> &_tokens) : tokens(_tokens) { }
};

// |int a, b, c|
// Também usado para declaração de funções: |int func|(int a, int b)
struct Declaracao: Comando{
    std::vector<Token> vars;
    tipo t;
    Declaracao() { }
    Declaracao(tipo _t, std::vector<Token> &_vars) : t(_t), vars(_vars) { }
};

// |a = 2|
struct Atribuicao: Comando {
    Token var;
    Expressao expr;
    Atribuicao() { }
    Atribuicao(Token &_var, Expressao _expr) : var(_var), expr(_expr) { }
};

// Argumentos para criação de funções: |(int a, float b)|
struct Argumentos: Comando {
    std::vector<Expressao> args;
    std::vector<Tipo> tipos; // tipos dos argumentos
    Argumentos(std::vector<Expressao> &_args, std::vector<Tipo> _tipos) : args(_args), tipos(_tipos) { }
};

// |return x+2|
struct Return: Comando {
    Expressao expr;
    Return() { }
    Return(Expressao _expr) : expr(_expr) { }
};

// |if(x == 5)|
struct If: Comando {
    Expressao cond;
    If(Expressao _cond) : cond(_cond) { }
};

// |else|
struct Else: Comando { };

// |for(int i = 0; i < 5; i++)|
// Formato: for(decl; expr1; expr2)
struct For: Comando {
    Atribuicao decl;
    Expressao expr1;
    Expressao expr2;
    For(Atribuicao _decl, Expressao _expr1, Expressao _expr2) : decl(_decl), expr1(_expr1), expr2(_expr2) { }
};

// |while(true)|
struct While: Comando {
    Expressao cond;
    While(Expressao _cond) : cond(_cond) { }
};

// |{|
struct AbreEscopo { };

// |}|
struct FechaEscopo { };

#endif
