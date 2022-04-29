#include <vector>

using namespace std;

struct Token { };
struct State { };

/**********************************************/

enum tipo { _int, _float, _void };

struct Comando {
      Comando();
      virtual void Update(State& state);
};

struct Tipo: Comando {
    tipo t;
    Tipo(tipo _t) : t(_t) { }
};

struct Expressao: Comando {
    vector<Token> tokens;
    Expressao() { }
    Expressao(vector<Token> &_tokens) : tokens(_tokens) { }
};

struct Declaracao: Comando{
    vector<Token> vars;
    tipo t;
    Declaracao() { }
    Declaracao(tipo _t, vector<Token> &_vars) : t(_t), vars(_vars) { }
};

struct Atribuicao: Comando {
    Token var;
    Expressao expr;
    Atribuicao() { }
    Atribuicao(Token &_var, Expressao _expr) : var(_var), expr(_expr) { }
};

struct Funcao: Comando {
    vector<Expressao> args;
    vector<Tipo> tipos;
    Funcao(vector<Expressao> &_args, vector<Tipo> _tipos) : args(_args), tipos(_tipos) { }
};

struct Return: Comando {
    Expressao expr;
    Return() { }
    Return(Expressao _expr) : expr(_expr) { }
};

struct If: Comando {
    Expressao cond;
    If(Expressao _cond) : cond(_cond) { }
};

struct Else: Comando { };

struct For: Comando {
    Atribuicao decl;
    Expressao expr1;
    Expressao expr2;
    For(Atribuicao _decl, Expressao _expr1, Expressao _expr2) : decl(_decl), expr1(_expr1), expr2(_expr2) { }
};

struct While: Comando {
    Expressao cond;
    While(Expressao _cond) : cond(_cond) { }
};

struct AbreEscopo { };

struct FechaEscopo { };
