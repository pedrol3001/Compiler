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
%token ID

%token INTEGER FLOAT

%token SEMICOLON COMMA

%token ASSIGN

%token SUM SUB DIV MUL

%token LESS LESSEQUAL GREATER GREATEREQUAL EQUAL NOTEQUAL

%right LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%right IF ELSE WHILE RETURN

%token VOID INT

%token OUTPUT INPUT

%%

program: declaration-list ;

declaration-list: declaration-list declaration | declaration ;

declaration: var-declaration | fun-declaration ;

var-declaration: type-specifier ID SEMICOLON | type-specifier ID LBRACKET INTEGER RBRACKET SEMICOLON ;

type-specifier: INT | VOID ;

fun-declaration: type-specifier ID LPAREN params RPAREN compound-stmt ;

params: param-list | VOID ;

param-list: param-list COMMA param | param ;

param: type-specifier ID | type-specifier ID LBRACKET RBRACKET ;

compound-stmt: LBRACE local-declarations statement-list RBRACE ;

local-declarations: local-declarations var-declaration | %empty ;

statement-list: statement-list statement | %empty ;

statement: expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt | output-stmt | input-stmt;

input-stmt: INPUT LPAREN ID RPAREN SEMICOLON ;

output-stmt: OUTPUT LPAREN expression RPAREN SEMICOLON ;

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
