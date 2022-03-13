%{
#include <stdio.h>
int yylex();
void yyerror(char *s);

unsigned long long erros = 0;

extern FILE *yyin;
extern FILE *yyout;

void debug(char *s) {
  printf("%s\n", s);
}
%}

/* declare tokens */
%token IDENTIFIER

%token INTEGER FLOAT

%token SEMICOLON COMMA

%token ASSIGN

%token SUM SUB DIV MUL

%token LESS LESSEQUAL GREATER GREATEREQUAL EQUAL NOTEQUAL

%token IF ELSE WHILE RETURN

%token VOID INT

%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%%

program: declaration-list { YYACCEPT; }  ;

declaration-list: declaration-list declaration | declaration ;

declaration: var-declaration | fun-declaration ;

var-declaration: type-specifier IDENTIFIER SEMICOLON | type-specifier IDENTIFIER LBRACKET INTEGER RBRACKET SEMICOLON ;

type-specifier: INT | VOID ;

fun-declaration: type-specifier IDENTIFIER LPAREN params RPAREN compound-stmt ;

params: param-list VOID ;

param-list: param-list COMMA param | param ;

param: type-specifier IDENTIFIER | type-specifier IDENTIFIER LBRACKET RBRACKET ;

compound-stmt: LBRACE local-declarations statement-list RBRACE ;

local-declarations: local-declarations var-declaration | %empty ;

statement-list: statement-list statement | %empty ;

statement: expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt ;

expression-stmt: expression SEMICOLON | SEMICOLON ;

selection-stmt: IF LPAREN expression RPAREN statement | IF LPAREN expression RPAREN statement ELSE statement ;

iteration-stmt: WHILE LPAREN expression RPAREN statement ;

return-stmt: RETURN SEMICOLON | RETURN expression SEMICOLON ;

expression: var ASSIGN expression | simple-expression ;

var: IDENTIFIER | IDENTIFIER LBRACKET expression RBRACKET ;

simple-expression: additive-expression relop additive-expression | additive-expression ;

relop: LESSEQUAL | LESS | GREATER | GREATEREQUAL | EQUAL | NOTEQUAL;

additive-expression: additive-expression addop term | term ;

addop: SUM | SUB ;

term: term mulop factor | factor ;

mulop: MUL | DIV ;

factor: LPAREN expression RPAREN | var | call | NUM ;

call: IDENTIFIER LPAREN RPAREN | IDENTIFIER LPAREN args RPAREN ;

args: arg-list ;

arg-list: arg-list COMMA expression | expression | %empty;

NUM: INTEGER | FLOAT ;

%%
void main(int argc, char **argv){
  ++argv; --argc; 	    /* abre arquivo de entrada se houver */
  if(argc > 0)
      yyin = fopen(argv[0],"rt");
  else
      yyin = stdin;    /* cria arquivo de saida se especificado */
  if(argc > 1)
      yyout = fopen(argv[1],"wt");
  else
      yyout = stdout;

  yyparse();
  printf("Finished: %llu erros", erros);

  fclose(yyin);
  fclose(yyout);

}

void yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
  erros++;
}
