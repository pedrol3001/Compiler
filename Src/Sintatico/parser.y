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
#include "Gerador/Gerador.h"

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

	string const_name(Token t){
		return ((StrAtt*) tabsim[t]["StrAtt"])->str;
	}

	struct Simb {
		string nome;
		int tipo; // 0 = int, 1 = void
		int escopo;
		int natureza; // 0 = var, 1 = funcao, 2 = array
		string tamanho = "1";
		bool usado = false;
	};

	int erros_semantico = 0, escopo = 0;

	struct Gerenciador {
		vector<Simb> vars;
		vector<Simb> globais;

		Gerenciador(){
			vars.push_back(Simb{"input", 1, 0, 1});
			vars.push_back(Simb{"output", 1, 0, 1});
		}

		void adicionar(string nome, int tipo, int natureza, int escopo, string tamanho){
			int st = this->existe(nome);
			if(st != -1 && st < escopo){
				std::cout << "Aviso: variável \"" << nome << "\" sendo substituída por variável local." << std::endl;
			}else if(st == escopo){
				std::cout << "Erro: variável \"" << nome << "\" sendo redeclarada." << std::endl;
			}

			Simb s{nome, tipo, escopo, natureza, tamanho};

			if(tipo == 285)
				tipo = 0; // int
			else if(tipo == 284)
				tipo = 1; // void

			if(natureza == 0 && tipo == 1){ // variavel com tipo void
				std::cout << "Erro: variável \"" << nome << "\" não pode ser do tipo void." << std::endl;
				erros_semantico++;
			}

			this->vars.push_back(s);
			if(s.escopo == 0)
				this->globais.push_back(s);
			else
				cout << "aloca " << s.nome << " " << s.tamanho << "\n";
		}
		int existe(string nome){
			for(auto it = vars.begin(); it != vars.end(); it++)
				if(it->nome == nome) return it->escopo;
			return -1;
		}
		bool verificar(string nome, int tipo){
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

					it->usado = true;
					return true;
				};
			}
			std::cout << "Erro: variável \"" << nome << "\" não declarada." << std::endl;
			erros_semantico++;
			return false;
		}
		void desalocar(){
			while(!vars.empty() && vars.back().escopo > escopo){
				cout << "desaloca " << vars.back().nome << " " << vars.back().tamanho << std::endl;
				vars.pop_back();
			}
		}
		void mostrar_globais(){
			cout << "=============================================\n";
			for(auto it = globais.begin(); it != globais.end(); it++)
				cout << "global: " << it->nome << " " << it->tamanho << std::endl; 
		}
	};

	struct Suporte{
		int temp_index = 0;
		
		string obter_temporario(){
			return "t" + std::to_string(temp_index++);
		}
		void reset_index(){
			temp_index = 0;
		}

	};

	Gerenciador tabela;
	Suporte suporte;
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

program: declaration-list { tabela.mostrar_globais(); };

declaration-list: declaration-list declaration | declaration ;

declaration: var-declaration | fun-declaration ;

var-declaration: type-specifier ID SEMICOLON {
		tabela.adicionar(token_name($2), $1.tipo, 0, escopo, "1");
	}
	| type-specifier ID LBRACKET C_INT RBRACKET SEMICOLON {
		tabela.adicionar(token_name($2), $1.tipo, 2, escopo, token_name($4));
	};

type-specifier: INT | VOID ;

fun-declaration: type-specifier ID {
		tabela.adicionar(token_name($2), $1.tipo, 1, escopo, "1");
	} open-esc LPAREN params RPAREN compound-stmt close-esc ;

open-esc: %empty { escopo++; } ;
close-esc: %empty { escopo--; tabela.desalocar(); } ;

params: param-list | VOID ;

param-list: param-list COMMA param | param ;

param: type-specifier ID {
		tabela.adicionar(token_name($2), $1.tipo, 0, escopo, "1");
	}
	| type-specifier ID LBRACKET RBRACKET {
		tabela.adicionar(token_name($2), $1.tipo, 2, escopo, "1"); // mudar tamanho
	};

compound-stmt: LBRACE open-esc local-declarations statement-list close-esc RBRACE ;

local-declarations: local-declarations var-declaration | %empty ;

statement-list: statement-list statement | %empty ;

statement: expression-stmt | compound-stmt | selection-stmt | iteration-stmt | input-stmt | output-stmt | return-stmt ;

input-stmt: INPUT var SEMICOLON { /*Gerar intermediario Input() */  } ;

output-stmt: OUTPUT var SEMICOLON;

expression-stmt: expression SEMICOLON | SEMICOLON ;

selection-stmt: IF LPAREN expression RPAREN statement | IF LPAREN expression RPAREN statement ELSE statement ;

iteration-stmt: WHILE LPAREN expression RPAREN statement ;

return-stmt: RETURN SEMICOLON | RETURN expression SEMICOLON ;

expression: var ASSIGN expression {
	cout << const_name($1) << " = " << const_name($3) << endl;	
}
	| simple-expression ;

var: ID { 
		// Analise semantica
		tabela.verificar(token_name($1), 0); 
		$$=$1;
	} | ID LBRACKET expression RBRACKET { 
		// Analise semantica
		tabela.verificar(token_name($1), 2); 
		// Converter vetor [$3 = expression] para:
		/*cout << "Simplificar x = v[n]" << endl;
		Token temp1 = tabsim.insert(ID);
		tabsim[temp].insert(new StrAtt("ArrayTemp1"));
		cout << '\t' << const_name(temp1) << " = " << const_name($1) << " + " << const_name($3) << " (temp1 = var + n)"; //Adicao(temp1,$1,$3);
		cout << '\t' <<;
			// temp1 = criar novo token
			// Adicao(temp1,$1,$3) temp1 = vetor(ponteiro) + expression 
		*/	// temp2 = *temp1
	} ;



simple-expression: additive-expression relop additive-expression {
	Token token = tabsim.insert(285);
	string t = suporte.obter_temporario();
	//tabsim[token].insert((Atributo*)(new IdVal(t)));
	tabsim[token].insert((Atributo*)(new StrAtt(t)));
	cout << const_name(token) << " = " << const_name($1) << " " << const_name($2) << " " << const_name($3) << endl;
	$$ = token;
}
	| additive-expression ;

relop: LESS | LESSEQUAL | GREATER | GREATEREQUAL | EQUAL | NOTEQUAL;

additive-expression: additive-expression addop term {
	Token token = tabsim.insert(285);
	string t = suporte.obter_temporario();
	//tabsim[token].insert((Atributo*)(new IdVal(t)));
	tabsim[token].insert((Atributo*)(new StrAtt(t)));

	cout << const_name(token) << " = " << const_name($1) << " " << const_name($2) << " " << const_name($3) << endl;
	$$ = token;
}
	| term ;

addop: SUM | SUB ;

term: term mulop factor {
	Token token = tabsim.insert(285);
	string t = suporte.obter_temporario();
	//tabsim[token].insert((Atributo*)(new IdVal(t)));
	tabsim[token].insert((Atributo*)(new StrAtt(t)));

	cout << const_name(token) << " = " << const_name($1) << " " << const_name($2) << " " << const_name($3) << endl;
	$$ = token;
}
	| factor {
		cout << "factor: " << const_name($1) << endl; // testando
	};

mulop: MUL | DIV ;

factor: LPAREN expression RPAREN | var | call | NUM ;

call: ID LPAREN args RPAREN { 
	tabela.verificar(token_name($1), 1); 

} {/* call() ;*/};

args: arg-list | %empty ;

arg-list: arg-list COMMA expression {
	cout << "param " << const_name($3) << endl;	
}
	| expression | %empty;

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
