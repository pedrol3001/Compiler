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
%%
{Letra}({Digito}|{Letra}|_)* {fprintf(yyout,"[Identificador, %s]\n",yytext);}

{Sinal}*{Digito}+		{fprintf(yyout,"[Inteiro, %s]\n",yytext);}

{Sinal}*{Digito}+\.{Digito}*	{fprintf(yyout,"[Ponto flutuante, %s]\n",yytext);}

\"((\\\")|[^\"\n])*\"      {fprintf(yyout,"[String, %s]\n",yytext);}

[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]      {fprintf(yyout,"[Comentario, %s]\n",yytext);}

{Space}*				{;}

"/*"                      { BEGIN(INVALIDO_VARIAS_LINHAS); yyless(yyleng-2); }
<INVALIDO_VARIAS_LINHAS>(.|\n)+         { fprintf(yyout,"[Invalido, %s]\n",yytext); BEGIN(INITIAL); }

.                 { BEGIN(INVALIDO_UMA_LINHA); yyless(yyleng-1); }
<INVALIDO_UMA_LINHA>.+  { fprintf(yyout,"[Invalido, %s]\n",yytext); BEGIN(INITIAL); }

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
