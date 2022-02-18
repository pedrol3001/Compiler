%{
#include <stdio.h>
extern FILE *yyin;
extern FILE *yyout;
%}
/*Definitions*/
Digito [0-9]
Letra [a-zA-Z]
Sinal [-+]
Space [\n\t ]
NotSpace [^\n\t ]

%x INVALIDO_VARIAS_LINHAS
%x INVALIDO_UMA_LINHA

/*Rules*/
%{
	void match(char* tipo) {
		fprintf(yyout,"[%s: %s]\n",tipo,yytext);
	} 
%}
%%

"+" {
	match("Soma");
}

"-" {
	match("Subtracao");
}

"*" {
	match("Multiplicacao");
}

"/" {
	match("Divisao");
}

"<" {
	match("Menor");
}

">" {
	match("Maior");
}

"<=" {
	match("Menor ou igual");
}

">=" {
	match("Maior ou igual");
}

"==" {
	match("Igualdade");
}

"!=" {
	match("Diferenca");
}

"=" {
	match("Atribuicao");
}

";" {
	match("Ponto e virgula");
}

"," {
	match("Virgula");
}

"if" {
	match("if");
}

"else" {
	match("else");
}

"else if" {
	match("else if");
}

"return" {
	match("return");
}

"while" {
	match("while");
}

"void" {
	match("void");
}

{Letra}({Digito}|{Letra}|_)* {
	match("Identificador");
}

{Digito}+ {
	match("Inteiro");
}

({Digito}+"."{Digito}*)|({Digito}*"."{Digito}+) {
	match("Ponto flutuante");
}

\"(\\\"|[^\"\n])*\" {
	match("String");
}

"/*"[^*]*"*"+([^/][^*]*"*"+)*"/" {
	match("Comentario");
}

{Space}* {
	
}

"/*" { 
	BEGIN(INVALIDO_VARIAS_LINHAS); 
	yyless(yyleng-2); 
}
<INVALIDO_VARIAS_LINHAS>(.|\n)+ { 
	match("Invalido");
	BEGIN(INITIAL); 
}

. { 
	BEGIN(INVALIDO_UMA_LINHA); yyless(yyleng-1); 
}

<INVALIDO_UMA_LINHA>.+ { 
	match("Invalido");
	BEGIN(INITIAL); 
}

%%

int yywrap ();

/*User*/
int main(argc, argv)
    int argc;
    char **argv;
        {
        ++argv; --argc; 	    /* abre arquivo de entrada se houver */
        if(argc > 0)
            yyin = fopen(argv[0],"rt");
        else
            yyin = stdin;    /* cria arquivo de saida se especificado */
        if(argc > 1)
            yyout = fopen(argv[1],"wt");
        else
            yyout = stdout;
        yylex(); /* scanner gerado por Flex */
        fclose(yyin);
        fclose(yyout);
        }

int yywrap() {
	return 1;
}
