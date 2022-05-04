%code requires {
#include "Lexico/Lexico.h"
#include "Semantico/Semantico.h"
#include <vector>
#include <iostream>
#include <memory>
#include "Token/Token.h"

using namespace std;

class Token;
//class Value;
}

%{
#include <iostream>
#include <stdio.h>
#include "Sintatico/Sintatico.h"
#include "Lexico/Lexico.h"
#include "Semantico/Semantico.h"

unsigned long long erros = 0;
using namespace std;

// Implementacao no final do .y
void debug(string s);
void yyerror(Lexico& lexico, Semantico& semantico, string s);
int yylex(Lexico& lexico);

%}

// yylval ======================================================== 

%{
	struct Value {
		vector<Token> tokens;
		Value(): tokens() {}
		Value(Token& token): tokens(1,token) {}
		Value(vector<Token>& _tokens): tokens(_tokens) {}
		vector<Token> operator()() {return tokens;}
	};
	
	Value operator,(Value& l, Value& r) {
		Value ret(l);
		ret.tokens.insert(ret.tokens.end(),r.tokens.begin(),r.tokens.end());
		return ret;
	}

	TabSim& tabsim = TabSim::getInstance();

	string token_name(Token t){
		return ((IdVal*) tabsim[t]["IdVal"])->val;
	}

	struct Simb {
		string nome;
		int tipo; // 0 = int, 1 = void
		int escopo;
		int natureza; // 0 = var, 1 = funcao, 2 = array
		bool usado = false;
	};

	int erros_semantico = 0, escopo = 0;

	struct Gerenciador {
		vector<Simb> vars;

		Gerenciador(){
			vars.push_back(Simb{"input", 1, 0, 1});
			vars.push_back(Simb{"output", 1, 0, 1});
		}

		void adicionar(string nome, int tipo, int natureza, int escopo){

			this->verificar_escopo();

			std::cout << "Adicionando " << nome << " no escopo " << escopo << std::endl;

			int st = this->existe(nome);
			if(st != -1 && st < escopo){
				std::cout << "Aviso: variável \"" << nome << "\" sendo substituída por variável local." << std::endl;
			}else if(st == escopo){
				std::cout << "Erro: variável \"" << nome << "\" sendo redeclarada." << std::endl;
			}

			Simb s{nome, tipo, escopo, natureza};

			if(tipo == 285)
				tipo = 0; // int
			else if(tipo == 284)
				tipo = 1; // void

			if(natureza == 0 && tipo == 1){ // variavel com tipo void
				std::cout << "Erro: variável \"" << nome << "\" não pode ser do tipo void." << std::endl;
				erros_semantico++;
			}

			this->vars.push_back(s);
			std::cout << nome << " adicionado com sucesso." << std::endl;
		}
		int existe(string nome){
			for(auto it = vars.begin(); it != vars.end(); it++)
				if(it->nome == nome) return it->escopo;
			return -1;
		}
		bool verificar(string nome, int tipo){
			this->verificar_escopo();
			for(auto it = vars.begin(); it != vars.end(); it++){
				if(it->nome == nome){
					if(it->natureza != 1 && tipo == 1){ // não-função sendo usada como função
						std::cout << "Erro: variável \"" << nome << "\" não pode ser usada como uma função." << std::endl;
						erros_semantico++;
						return false;
					}
					if(it->natureza == 0 && tipo == 2){ // variável normal sendo usada como array
						std::cout << "Erro: variável \"" << nome << "\" não pode ser usada como um array." << std::endl;
						erros_semantico++;
						return false;
					}
					
					return true;
				};
			}
			std::cout << "Erro: variável \"" << nome << "\" não declarada." << std::endl;
			erros_semantico++;
			return false;
		}
		void verificar_escopo(){
			while(!vars.empty() && vars.back().escopo > escopo)
				vars.pop_back();
		}
	};

	Gerenciador tabela;

%}

//%define api.value.type {Value}
%define api.value.type {Token}

/* declare tokens */
	// Usado para debug
%token NOT_INITIALIZED

	// Comentários
%token COMMENT

	// Constantes
%token  C_STRING C_CHAR C_INT C_FLOAT

// Estruturas
	// Separadores
%token	SEMICOLON COMMA
	// Invólucros
%right	LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

// Palavras reservadas
	// Tipos
%token		VOID
	// Letras
%token 	STRING CHAR
	// Números
%token 	INT FLOAT DOUBLE
	// Comandos
%token		RETURN OUTPUT INPUT
	// Construções
	
%right 	IF ELSE
 	// Loop
%token 	WHILE
	// Identificador
%token	ID
// Operações
	// Atribuição
%token		ASSIGN
	// Operações aritméticas
%token		SUM SUB DIV MUL
	// Comparações
%token		LESS LESSEQUAL GREATER GREATEREQUAL EQUAL NOTEQUAL

// Tipos das regras ==============================================

// Outros parametros =============================================

%define parse.error detailed

%lex-param {Lexico &lexico}
%parse-param {Lexico &lexico} {Semantico& semantico}

%%

program: declaration-list ;

declaration-list: declaration-list declaration | declaration ;

declaration: var-declaration | fun-declaration ;

var-declaration: type-specifier ID SEMICOLON {
		tabela.adicionar(token_name($2), $1.tipo, 0, escopo);
	}
	| type-specifier ID LBRACKET C_INT RBRACKET SEMICOLON {
		tabela.adicionar(token_name($2), $1.tipo, 2, escopo);
	};

type-specifier: INT | VOID ;

fun-declaration: type-specifier ID {
		tabela.adicionar(token_name($2), $1.tipo, 1, escopo);
	} open-esc LPAREN params RPAREN compound-stmt close-esc ;

open-esc: %empty { escopo++; } ;
close-esc: %empty { escopo--; } ;

params: param-list | VOID ;

param-list: param-list COMMA param | param ;

param: type-specifier ID {
		tabela.adicionar(token_name($2), $1.tipo, 0, escopo);
	}
	| type-specifier ID LBRACKET RBRACKET {
		tabela.adicionar(token_name($2), $1.tipo, 2, escopo);
	};

compound-stmt: LBRACE open-esc local-declarations statement-list close-esc RBRACE ;

local-declarations: local-declarations var-declaration | %empty ;

statement-list: statement-list statement | %empty ;

statement: expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt ;

expression-stmt: expression SEMICOLON | SEMICOLON ;

selection-stmt: IF LPAREN expression RPAREN statement | IF LPAREN expression RPAREN statement ELSE statement ;

iteration-stmt: WHILE LPAREN expression RPAREN statement ;

return-stmt: RETURN SEMICOLON | RETURN expression SEMICOLON ;

expression: var ASSIGN expression | simple-expression ;

var: ID { tabela.verificar(token_name($1), 0); } 
	| ID LBRACKET expression RBRACKET { tabela.verificar(token_name($1), 2); } ;

simple-expression: additive-expression relop additive-expression | additive-expression ;

relop: LESS | LESSEQUAL | GREATER | GREATEREQUAL | EQUAL | NOTEQUAL;

additive-expression: additive-expression addop term | term ;

addop: SUM | SUB ;

term: 	term mulop factor {	
		//container.emplace_back(new Expressao($2()));	// TODO: eh um teste}
	} | factor;

mulop: MUL | DIV ;

factor: LPAREN expression RPAREN | var | call | NUM ;

call: ID LPAREN args RPAREN { tabela.verificar(token_name($1), 1); } ;

args: arg-list | %empty ;

arg-list: arg-list COMMA expression | expression | %empty;

NUM: C_INT | C_FLOAT ;

%%
void yyerror(Lexico& lexico, Semantico& semantico , string s) {
	fprintf(stderr, "error: %s\n", s.c_str());
	erros++;
}

int yylex(Lexico& lexico){
	Token token;
	if(lexico >> token) {
		//yylval = Value(token);
		yylval = token;
		return token();
	} else {
		return YYEOF;
	}
}	

void debug(string s) {
  printf("%s\n", s.c_str());
}


/*
void main(int argc, char **argv){
  ++argv; --argc; 	    // abre arquivo de entrada se houver
  if(argc > 0)
      yyin = fopen(argv[0],"rt");
  else
      yyin = stdin;    // cria arquivo de saida se especificado
  if(argc > 1)
      yyout = fopen(argv[1],"wt");
  else
      yyout = stdout;

  yyparse();
  printf("Finished: %llu erros", erros);

  fclose(yyin);
  fclose(yyout);

}
*/
