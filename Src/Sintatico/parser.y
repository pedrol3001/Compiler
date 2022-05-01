%code requires {
#include "../Lexico/Lexico.h"
#include "../Semantico/Semantico.h"
#include <vector>
using namespace std;
}

%{
#include <iostream>
#include <stdio.h>
#include "Sintatico.h"
#include "../Lexico/Lexico.h"
#include "../Semantico/Semantico.h"

unsigned long long erros = 0;
using namespace std;
	
// Implementacao no final do .y
void debug(string s);
void yyerror(Lexico& lexico, std::vector<Bloco*> container, string s);
int yylex(Lexico& lexico);

%}

%define parse.error detailed

%lex-param {Lexico &lexico}
%parse-param {Lexico &lexico} {std::vector<Bloco*> container}

/* declare tokens */
// Usado para debug
%token NOT_INITIALIZED

// Comentários
%token COMMENT

// Constantes
%token  C_STRING C_CHAR C_INT C_FLOAT

// Palavras reservadas
	// Tipos
%token		VOID
		// Letras
%token 	STRING CHAR
		// Números
%token 	INT FLOAT DOUBLE 
	// Comandos
%token	RETURN OUTPUT INPUT
	// Construções
%token	IF ELSE WHILE
	
// Identificador
%token	ID

// Estruturas
	// Separadores
%token	SEMICOLON COMMA
	// Invólucros
%token	LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

// Operações
	// Atribuição
%token	ASSIGN
	// Operações aritméticas
%token	SUM SUB DIV MUL
	// Comparações
%token	LESS LESSEQUAL GREATER GREATEREQUAL EQUAL NOTEQUAL

%%

program: declaration-list ;

declaration-list: declaration-list declaration | declaration ;

declaration: var-declaration | fun-declaration ;

var-declaration: type-specifier ID SEMICOLON | type-specifier ID LBRACKET C_INT RBRACKET SEMICOLON ;

type-specifier: INT | VOID ;

fun-declaration: type-specifier ID LPAREN params RPAREN compound-stmt ;

params: param-list | VOID ;

param-list: param-list COMMA param | param ;

param: type-specifier ID | type-specifier ID LBRACKET RBRACKET ;

compound-stmt: LBRACE local-declarations statement-list RBRACE ;

local-declarations: local-declarations var-declaration | %empty ;

statement-list: statement-list statement | %empty ;

statement: expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt ;

expression-stmt: expression SEMICOLON | SEMICOLON ;

selection-stmt: IF LPAREN expression RPAREN statement | IF LPAREN expression RPAREN statement ELSE statement ;

iteration-stmt: WHILE LPAREN expression RPAREN statement ;

return-stmt: RETURN SEMICOLON | RETURN expression SEMICOLON ;

expression: var ASSIGN expression | simple-expression ;

var: ID | ID LBRACKET expression RBRACKET ;

simple-expression: additive-expression relop additive-expression | additive-expression ;

relop: LESS | LESSEQUAL | GREATER | GREATEREQUAL | EQUAL | NOTEQUAL;

additive-expression: additive-expression addop term | term ;

addop: SUM | SUB ;

term: term mulop factor | factor ;

mulop: MUL | DIV ;

factor: LPAREN expression RPAREN | var | call | NUM ;

call: ID LPAREN args RPAREN ;

args: arg-list | %empty ;

arg-list: arg-list COMMA expression | expression | %empty;

NUM: C_INT | C_FLOAT ;

%%
void yyerror(Lexico& lexico, std::vector<Bloco*> container, string s) {
	fprintf(stderr, "error: %s\n", s.c_str());
	erros++;
}

int yylex(Lexico& lexico){
	Token token;
	if(lexico >> token)
		return token();
	else
		return YYEOF;
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
