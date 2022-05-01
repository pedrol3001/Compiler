#ifndef Bloco_h
#define Bloco_h

#include <iostream>
#include <vector>
#include "../Token/Token.h"

class Semantico;

struct Bloco {
	std::vector<Token> tokens;	
	Bloco(); 
	Bloco(const std::vector<Token>& _tokens); // Tokens
	virtual ~Bloco();
	virtual bool analisar(Semantico& state);
	virtual void gerar(Semantico& state)=0;
	// Debug
	std::vector<Token> getTokens();
};

// |a|, |b|, |obj.x|, |*c|, |&d|, |v[5+7]|, |a * f(b) + obj.x|, |f(g(h(2)))|
struct Expressao: Bloco {
    	Expressao(const std::vector<Token> &notacaoPolonesaReversa);
	void gerar(Semantico& state);
};

struct Nada: Bloco {
    Nada();
};


/*

// |int|, |float|, |void|
struct Tipo: Bloco {
    tipo t;
    Tipo(tipo _t) : t(_t) { }
};

// |int a, b, c|
// Também usado para declaração de funções: |int func|(int a, int b)
struct Declaracao: Bloco{
    std::vector<Token> vars;
    tipo t;
    Declaracao() { }
    Declaracao(tipo _t, std::vector<Token> &_vars) : t(_t), vars(_vars) { }
};

// |a = 2|
struct Atribuicao: Bloco {
    Token var;
    Expressao expr;
    Atribuicao() { }
    Atribuicao(Token &_var, Expressao _expr) : var(_var), expr(_expr) { }
};

// Argumentos para criação de funções: |(int a, float b)|
struct Argumentos: Bloco {
    std::vector<Expressao> args;
    std::vector<Tipo> tipos; // tipos dos argumentos
    Argumentos(std::vector<Expressao> &_args, std::vector<Tipo> _tipos) : args(_args), tipos(_tipos) { }
};

// |return x+2|
struct Return: Bloco {
    Expressao expr;
    Return() { }
    Return(Expressao _expr) : expr(_expr) { }
};

// |if(x == 5)|
struct If: Bloco {
    Expressao cond;
    If(Expressao _cond) : cond(_cond) { }
};

// |else|
struct Else: Bloco { };

// |for(int i = 0; i < 5; i++)|
// Formato: for(decl; expr1; expr2)
struct For: Bloco {
    Atribuicao decl;
    Expressao expr1;
    Expressao expr2;
    For(Atribuicao _decl, Expressao _expr1, Expressao _expr2) : decl(_decl), expr1(_expr1), expr2(_expr2) { }
};

// |while(true)|
struct While: Bloco {
    Expressao cond;
    While(Expressao _cond) : cond(_cond) { }
};

// |{|
struct AbreEscopo { };

// |}|
struct FechaEscopo { };
*/

#endif
