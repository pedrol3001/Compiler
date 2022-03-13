// 1. program → declaration-list
// 2. declaration-list → declaration-list declaration | declaration
// 3. declaration → var-declaration | fun-declaration
// 4. var-declaration → type-specifier ID ; | type-specifier ID [ NUM ] ;
// 5. type-specifier → int | void
// 6. fun-declaration → type-specifier ID ( params ) compound-stmt
// 7. params → param-list | void
// 8. param-list → param-list , param | param
// 9. param → type-specifier ID | type-specifier ID [ ]
// 10. compound-stmt → { local-declarations statement-list }
// 11. local-declarations → local-declarations var-declarations | empty
// 12. statement-list → statement-list statement | empty
// 13. statement → expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt
// 14. expression-stmt → expression ; | ;
// 15. selection-stmt → if ( expression ) statement | if ( expression ) statement else statement
// 16. iteration-stmt → while ( expression ) statement
// 17. return-stmt → return ; | return expression ;
// 18. expression → var = expression | simple-expression
// 19. var → ID | ID [ expression ]
// 20. simple-expression → additive-expression relop additive-expression | additive-expression
// 21. relop → <= | < | > | >= | == | !=
// 22. additive-expression → additive-expression addop term | term
// 23. addop → + | -
// 24. term → term mulop factor | factor
// 25. mulop → * | /
// 26. factor → ( expression ) | var | call | NUM
// 27. call → ID ( args )
// 28. args → arg-list | empty
// 29. arg-list → arg-list , expression | expression
// Keywords: else if int return void while
// Special symbols: + - * / < <= > >= == != = ; , ( ) [ ] { } /* */
// ID = letter letter*
// NUM = digit digit*
// letter = a | .. | z | A | .. | Z
// digit = 0 | .. | 9
// Comments: /* ... */

%{
#include <stdio.h>
int yylex();
void yyerror(char *s);

unsigned long long erros = 0;

extern FILE *yyin;
extern FILE *yyout;
%}

/* declare tokens */
%token ID

%token INTEGER FLOAT

%token SEMICOLON COMMA

%token ASSIGN

%token SUM SUB DIV MUL

%token LESS LESSEQUAL GREATER GREATEREQUAL EQUAL NOTEQUAL

%token IF ELSE WHILE RETURN

%token VOID INT

%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%%

program:
  | declaration-list { printf("OK"); }
  ;

declaration-list:
  | declaration-list declaration
  | declaration
  ;

declaration:
  | var-declaration
  | fun-declaration
  ;

var-declaration:
  | type-specifier ID SEMICOLON
  | type-specifier ID LBRACKET INTEGER RBRACKET SEMICOLON
  ;

type-specifier:
  | INT
  | VOID
  ;

fun-declaration:
  | type-specifier ID LPAREN params RPAREN compound-stmt
  ;

params:
  | param-list
  | VOID
  ;

param-list:
  | param-list COMMA param
  | param
  ;

param:
  | type-specifier ID
  | type-specifier ID LBRACKET RBRACKET
  ;

compound-stmt:
  | LBRACE local-declarations statement-list RBRACE
  ;

local-declarations: %empty
  | local-declarations var-declaration
  ;

statement-list: %empty
  | statement-list statement
  ;

statement:
  | expression-stmt
  | compound-stmt
  | selection-stmt
  | iteration-stmt
  | return-stmt
  ;

expression-stmt:
  | expression SEMICOLON
  | SEMICOLON
  ;

selection-stmt:
  | IF LPAREN expression RPAREN statement
  | IF LPAREN expression RPAREN statement ELSE statement
  ;

iteration-stmt:
  | WHILE LPAREN expression RPAREN statement
  ;

return-stmt:
  | RETURN SEMICOLON
  | RETURN expression SEMICOLON
  ;

expression:
  | var ASSIGN expression
  | simple-expression
  ;

var:
  | ID
  | ID LBRACKET expression RBRACKET
  ;

simple-expression:
  | additive-expression relop additive-expression
  | additive-expression
  ;

relop:
  | LESSEQUAL
  | LESS
  | GREATER
  | GREATEREQUAL
  | EQUAL
  | NOTEQUAL
  ;

additive-expression:
  | additive-expression addop term
  | term
  ;

addop:
  | SUM
  | SUB
  ;

term:
  | term mulop factor
  | factor
  ;

mulop:
  | MUL
  | DIV
  ;

factor:
  | LPAREN expression RPAREN
  | var
  | call
  | num
  ;

call:
  | ID LPAREN RPAREN
  | ID LPAREN args RPAREN
  ;

args: %empty
  | arg-list
  ;

arg-list:
  | arg-list COMMA expression
  | expression
  ;

num:
  | INTEGER
  | FLOAT
  ;

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
  fclose(yyin);
  fclose(yyout);

  printf("Finished: %llu", erros);
}

void yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
  erros++;
}
