%code requires {
#include "Lexico/Lexico.h"
#include "Semantico/Semantico.h"
#include "Token/Token.h"
}

%{
#include <iostream>
#include <stdio.h>
#include "Sintatico/Sintatico.h"
#include "Lexico/Lexico.h"
#include "Semantico/Semantico.h"
#include "Gerador/Gerador.h"
#include <cassert>

unsigned long long erros = 0;
using namespace std;

// Implementacao no final do .y
void debug(string s);
void yyerror(Lexico& lexico, Semantico& semantico, std::string s);
int yylex(Lexico& lexico);

TabSim& tabsim = TabSim::getInstance();

string tokenIdVal(Token t){
	assert(tabsim[t].has("IdVal"));
	return ((IdVal*) tabsim[t]["IdVal"])->val;
}

string tokenStrAtt(Token t){
	assert(tabsim[t].has("StrAtt"));
	return ((StrAtt*) tabsim[t]["StrAtt"])->str;
}

long long int tokenIntVal(Token t){
	assert(tabsim[t].has("IntVal"));
	return ((IntVal*) tabsim[t]["IntVal"])->val;
}

bool isTemp(Token t) {
	return tabsim[t].has("IsTemp");
}


void updateTabSim(Token t, Semantico& semantico) {
	if(isTemp(t)) {
		tabsim[t].insert((Atributo*) new VarLocal(1));
		return;
	}

	if(semantico.tabela.existe(tokenIdVal(t))) {
		Simb simb = semantico.tabela[tokenIdVal(t)];
		if(simb.escopo==GLOBAL) {
			if(!tabsim[t].has("VarGlobal"))
				tabsim[t].insert((Atributo*) new VarGlobal(simb.tamanho));
		} else {
			if(!tabsim[t].has("VarLocal"))
				tabsim[t].insert((Atributo*) new VarLocal(simb.tamanho));
		}
	}
}
	
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
%}

//%define api.value.type {Value}
%define api.value.type {Token}

/* declare tokens */

// Usado para debug
%token NOT_INITIALIZED

// Usado na analise semnatica
%token TEMP

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

program: declaration_list { 
	semantico.tabela.mostrar_globais();
	semantico.analisar();	// Setar como ok
};

declaration_list: declaration_list declaration | declaration ;

declaration: var_declaration | fun_declaration ;

var_declaration: type_specifier ID SEMICOLON {
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::VAR, semantico.escopo, 1);
	} | type_specifier ID LBRACKET C_INT RBRACKET SEMICOLON {
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::ARRAY, semantico.escopo, tokenIntVal($4));
	};

type_specifier: INT | VOID ;

fun_declaration: type_specifier ID {
		
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo,  Simb::Nat::FUNCAO, semantico.escopo, 1);
		//semantico.code.emplace_back(new Label());
	} open_esc LPAREN params RPAREN compound_stmt close_esc ;

open_esc: %empty { semantico.escopo++; } ;
close_esc: %empty { semantico.escopo--; semantico.tabela.remover(); } ;

params: param_list | VOID ;

param_list: param_list COMMA param | param ;

param: type_specifier ID {
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::VAR, semantico.escopo, 1);
	}
	| type_specifier ID LBRACKET RBRACKET {
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::ARRAY, semantico.escopo, 1); // mudar tamanho
	};

compound_stmt: LBRACE open_esc local_declarations statement_list close_esc RBRACE ;

local_declarations: local_declarations var_declaration | %empty ;

statement_list: statement_list statement | %empty ;

statement: expression_stmt | compound_stmt | selection_stmt | iteration_stmt | input_stmt | output_stmt | return_stmt ;

input_stmt: INPUT var SEMICOLON { semantico.code.emplace_back(new Addr3::Read($2)); } ;

output_stmt: OUTPUT expression SEMICOLON { semantico.code.emplace_back(new Addr3::Print($2)); } ;

expression_stmt: expression SEMICOLON | SEMICOLON ;

selection_stmt: IF LPAREN expression RPAREN statement | IF LPAREN expression RPAREN statement ELSE statement ;

iteration_stmt: WHILE LPAREN expression RPAREN statement ;

return_stmt: RETURN SEMICOLON | RETURN expression SEMICOLON ;

expression: var ASSIGN expression {
		semantico.code.emplace_back(new Addr3::Atribuicao($1,$3));
	
		cout << tokenStrAtt($1) << " = " << tokenStrAtt($3) << endl;
		$$ = $1; // para retornar o resultado da igualdade
	} | simple_expression {
		semantico.tempGen.reset_index();
		$$ = $1;
	} ;

var: ID {	
		semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::VAR);
		updateTabSim($1,semantico);
		$$=$1;
		
	} | ID LBRACKET expression RBRACKET { 	
		semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::ARRAY); 
		updateTabSim($1,semantico);
		$$=$1;
	} ;



simple_expression: additive_expression relop additive_expression {
	Token token = semantico.tempGen.gerar();
	updateTabSim(token,semantico);
	
	cout << tokenStrAtt(token) << " = " << tokenStrAtt($1) << " " << tokenStrAtt($2) << " " << tokenStrAtt($3) << endl;
	$$ = token;
} | additive_expression ;

relop: LESS | LESSEQUAL | GREATER | GREATEREQUAL | EQUAL | NOTEQUAL;

additive_expression: additive_expression addop term {
	Token token = semantico.tempGen.gerar();
	updateTabSim(token,semantico);
	
	semantico.code.emplace_back(new Addr3::Adicao(token,$1,$3));	// token = $1 + $3
	cout << tokenStrAtt(token) << " = " << tokenStrAtt($1) << " " << tokenStrAtt($2) << " " << tokenStrAtt($3) << endl;
	$$ = token;
} | term ;

addop: SUM | SUB ;

term: term mulop factor {
	Token token = semantico.tempGen.gerar();
	updateTabSim(token,semantico);
	
	semantico.code.emplace_back(new Addr3::Multiplicacao(token,$1,$3) );	// token = $1 * $3
	cout << tokenStrAtt(token) << " = " << tokenStrAtt($1) << " " << tokenStrAtt($2) << " " << tokenStrAtt($3) << endl;
	$$ = token;
} | factor ;

mulop: MUL | DIV ;

factor: LPAREN expression RPAREN | var | call | NUM ;

call: ID LPAREN Addr3_BeginCall args RPAREN { 
	semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::VAR);

	Token token = semantico.tempGen.gerar();
	
	updateTabSim($1,semantico);
	updateTabSim(token,semantico);

	cout << tokenStrAtt(token) << " = " << "call " << tokenStrAtt($1) << endl;
	$$ = token;
};

Addr3_BeginCall: %empty { 
	cout << "begin call" << endl; 
	semantico.code.emplace_back(new Addr3::BeginCall);
} ;

args: arg_list | %empty ;

arg_list: arg_list COMMA expression addr3_param | addr3_param | %empty;
	
addr3_param: expression { 
	cout << "param " << tokenStrAtt($1) << endl;
}

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
