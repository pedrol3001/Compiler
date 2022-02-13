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

/*Rules*/
%%
{Letra}({Digito}|{Letra}|_)*{Space} {fprintf(yyout,"[Identificador, %s]\n",yytext);}//printf("Identificador\n"); {fprintf(yyout,"[Identificador, %s]\n",yytext);}

{Sinal}*{Digito}+{Space}		{fprintf(yyout,"[Inteiro, %s]\n",yytext);}//printf("Inteiro\n");

{Sinal}*{Digito}+\.{Digito}+{Space}	{fprintf(yyout,"[Ponto flutuante, %s]\n",yytext);}//printf("Ponto flutuante\n");

[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]      {fprintf(yyout,"[Comentario, %s]\n",yytext);} //\/\*.*\*\/{fprintf(yyout,"[Comentario, %s]\n",yytext);}//printf("Comentario\n");


{Space}*				/*Ignora*/

{NotSpace}+				{fprintf(yyout,"[Invalido, %s]\n",yytext);} //printf("Invalido\n");
%%

int yywrap ();

/*User*/
main(argc, argv)
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
