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

void aloca_global(Token token, Semantico &semantico) {
	//semantico.pseudoassembly.emplace_front("Aloca global " + tokenIdVal(token));
	semantico.init_code.emplace_back(new Addr3::AlocaGlobal(token));
}

void aloca_local(Token token, Semantico &semantico) {
	semantico.pseudoassembly.emplace_back("Aloca local " + tokenIdVal(token));
	semantico.code.emplace_back(new Addr3::Aloca(token));
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


	struct Zero {
		Token token;
		Zero();
		Token get();
	};
	Zero zero;
	
	Token condtoken;

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

program: {
	semantico.init_code.emplace_back(new Addr3::SetGlobal);

	condtoken = tabsim.insert(ID);
	string name = "$cond";
	tabsim[condtoken].insert((Atributo*)(new IdVal(name)));
	tabsim[condtoken].insert((Atributo*)(new StrAtt(name)));

	aloca_global(condtoken,semantico);
	semantico.tabela.adicionar(tokenIdVal(condtoken), INT, Simb::Nat::ARRAY, semantico.escopo, condtoken, 1, true);

	} declaration_list {
	
	semantico.init_code.emplace_back(new Addr3::SetLocal);

	semantico.tabela.salvar_globais(semantico.pseudoassembly);

	semantico.code.insert(semantico.code.begin(),semantico.init_code.begin(), semantico.init_code.end());
	semantico.analisar();	// Setar como ok
};

declaration_list: declaration_list declaration | declaration ;

declaration: var_declaration | fun_declaration ;

var_declaration: type_specifier ID SEMICOLON {
		if(semantico.escopo==GLOBAL)
			aloca_global($2,semantico);
		else
			aloca_local($2,semantico);
			
		semantico.pseudoassembly.emplace_back("declarar " + tokenStrAtt($1) + " " + tokenStrAtt($2));
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::VAR, semantico.escopo, $2, 1);

	} | type_specifier ID LBRACKET C_INT RBRACKET SEMICOLON {
		if(semantico.escopo==GLOBAL)
			aloca_global($2,semantico);
		else
			aloca_local($2,semantico);
			
		semantico.pseudoassembly.emplace_back("declarar " + tokenStrAtt($1) + " " + tokenStrAtt($2) + "[" + tokenStrAtt($4) + "]");
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::ARRAY, semantico.escopo, $2, tokenIntVal($4)+1);
	};

type_specifier: INT | VOID ;

fun_declaration: type_specifier ID {
		aloca_global($2, semantico);
		semantico.pseudoassembly.emplace_back("declarar " + tokenStrAtt($1) + " " + tokenStrAtt($2) + "()");
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo,  Simb::Nat::FUNCAO, semantico.escopo, $2, 1, true);
	} LPAREN open_esc params RPAREN compound_stmt close_esc ;

open_esc: %empty {
	semantico.pseudoassembly.emplace_back("// abrindo escopo");
	semantico.escopo++;
} ;
close_esc: %empty { 
	semantico.pseudoassembly.emplace_back("// fechando escopo");
	semantico.escopo--;

	for(auto it = semantico.tabela.variaveis.rbegin(); it != semantico.tabela.variaveis.rend(); it++) {
		if(!semantico.tabela.variaveis[it->first].empty() && semantico.tabela.variaveis[it->first].back().escopo > semantico.escopo){
			semantico.pseudoassembly.emplace_back("desalocar " + it->first);
			semantico.code.emplace_back(new Addr3::Desaloca(semantico.tabela.variaveis[it->first].back().token));
		}
	}

	semantico.tabela.remover();
	} ;

params: param_list | VOID ;

param_list: param_list COMMA param | param ;

param: type_specifier ID {
		aloca_local($2,semantico);
		semantico.pseudoassembly.emplace_back("parâmetro " + tokenStrAtt($1) + " " + tokenStrAtt($2));
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::VAR, semantico.escopo, $2, 1);
	}
	| type_specifier ID LBRACKET RBRACKET {
		aloca_local($2,semantico);
		semantico.pseudoassembly.emplace_back("parâmetro " + tokenStrAtt($1) + " " + tokenStrAtt($2) + "[]");
		semantico.tabela.adicionar(tokenIdVal($2), $1.tipo, Simb::Nat::ARRAY, semantico.escopo, $2, 1); // mudar tamanho
	};

compound_stmt: LBRACE open_esc local_declarations statement_list close_esc RBRACE ;

local_declarations: local_declarations var_declaration | %empty ;

statement_list: statement_list statement | %empty ;

statement: expression_stmt | compound_stmt | selection_stmt | iteration_stmt | input_stmt | output_stmt | return_stmt ;

input_stmt: INPUT var SEMICOLON {
		semantico.pseudoassembly.emplace_back("input " + tokenStrAtt($2));
		semantico.code.emplace_back(new Addr3::Read($2));
	};

output_stmt: OUTPUT expression SEMICOLON {
		semantico.pseudoassembly.emplace_back("output " + tokenStrAtt($2));
		semantico.code.emplace_back(new Addr3::Print($2));
	};

expression_stmt: expression SEMICOLON | SEMICOLON ;

selection_stmt: IF LPAREN condition RPAREN statement {
		semantico.pseudoassembly.emplace_back("Label " + tokenStrAtt($3));
		semantico.code.emplace_back(new Addr3::Label($3));
	}
	| IF LPAREN condition RPAREN statement ELSE {
		Token token = semantico.labelGen.gerar();

		semantico.pseudoassembly.emplace_back("Ir para " + tokenStrAtt(token));
		semantico.code.emplace_back(new Addr3::Goto(token));
		$$ = token;

		semantico.pseudoassembly.emplace_back("Label " + tokenStrAtt($3));
		semantico.code.emplace_back(new Addr3::Label($3));
	}
	statement {
		semantico.pseudoassembly.emplace_back("Label " + tokenStrAtt($7));
		semantico.code.emplace_back(new Addr3::Label($7));
	};

condition: open_esc expression {
		Token token = semantico.labelGen.gerar();

		semantico.pseudoassembly.emplace_back(tokenIdVal(condtoken) + " = " + tokenStrAtt($2));
		semantico.code.emplace_back(new Addr3::Atribuicao(condtoken,$2));

		$$ = token;
	} close_esc {
		semantico.pseudoassembly.emplace_back("Se " + tokenIdVal(condtoken) + " == " + tokenStrAtt(zero.get()) + " ir para " + tokenStrAtt($3));
		semantico.code.emplace_back(new Addr3::Beq(condtoken, zero.get(), $3));
		$$ = $3;
	};

iteration_stmt: WHILE {
		Token token = semantico.labelGen.gerar();

		semantico.pseudoassembly.emplace_back("Label " + tokenStrAtt(token));
		semantico.code.emplace_back(new Addr3::Label(token));

		$$ = token;
	}	
	LPAREN condition RPAREN statement {
		semantico.pseudoassembly.emplace_back("Ir para " + tokenStrAtt($2));
		semantico.code.emplace_back(new Addr3::Goto($2));

		semantico.pseudoassembly.emplace_back("Label " + tokenStrAtt($4));
		semantico.code.emplace_back(new Addr3::Label($4));

	};

return_stmt: RETURN SEMICOLON {
		semantico.pseudoassembly.emplace_back("retornar");
		semantico.code.emplace_back(new Addr3::Return());
	}
	| RETURN expression SEMICOLON {
		semantico.pseudoassembly.emplace_back("retornar " + tokenStrAtt($2));
		semantico.code.emplace_back(new Addr3::Return($2));
	};

expression: var ASSIGN expression {
		// usar atribuicao
		semantico.pseudoassembly.emplace_back(tokenIdVal($1) + " = " + tokenStrAtt($3));
		semantico.code.emplace_back(new Addr3::Atribuicao($1,$3));
		$$ = $3;
	} | ID LBRACKET expression RBRACKET ASSIGN expression {
		// usar storeinref
		semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::ARRAY);

		$$ = semantico.tabela.obter_token(tokenIdVal($1));
		semantico.tabela.marcar_usado(tokenIdVal($1));

		pair<bool, Token> p = semantico.tempGen.obter();
		bool criado = p.first;
		Token token = p.second;

		if(criado){
			aloca_local(token,semantico);
			semantico.tabela.adicionar(tokenIdVal(token), INT, Simb::Nat::ARRAY, semantico.escopo, token, 1, true);
		}
		
		semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = " + tokenStrAtt($$) + " + " + tokenStrAtt($3));
		semantico.code.emplace_back(new Addr3::Adicao(token,$$,$3));	// token = $1 + $3

		semantico.pseudoassembly.emplace_back("*" + tokenStrAtt(token) + " = " + tokenStrAtt($6));
		semantico.code.emplace_back(new Addr3::StoreInRef(token, $6)); // *token = token

		$$ = $6;
	}
	|  simple_expression ;

var: ID {	
		semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::VAR);
		
		$$ = semantico.tabela.obter_token(tokenIdVal($1));
		semantico.tabela.marcar_usado(tokenIdVal($1));

	} | ID LBRACKET expression RBRACKET { 	
		semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::ARRAY);

		$$ = semantico.tabela.obter_token(tokenIdVal($1));

		pair<bool, Token> p = semantico.tempGen.obter();
		bool criado = p.first;
		Token token = p.second;

		if(criado){
			aloca_local(token,semantico);
			semantico.tabela.adicionar(tokenIdVal(token), INT, Simb::Nat::ARRAY, semantico.escopo, token, 1, true);
		}
		
		semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = " + tokenStrAtt($$) + " + " + tokenStrAtt($3));
		semantico.code.emplace_back(new Addr3::Adicao(token,$$,$3));	// token = $1 + $3
		
		semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = *" + tokenStrAtt(token));
		semantico.code.emplace_back(new Addr3::LoadFromRef(token, token)); // token = *token

		$$ = token;
		semantico.tabela.marcar_usado(tokenIdVal($1));
	} ;



simple_expression: additive_expression relop additive_expression {
	pair<bool, Token> p = semantico.tempGen.obter();
	bool criado = p.first;
	Token token = p.second;

	if(criado){
		aloca_local(token,semantico);
		semantico.tabela.adicionar(tokenIdVal(token), INT, Simb::Nat::ARRAY, semantico.escopo, token, 1, true);
	}
	
	if($2.tipo == LESS)
		semantico.code.emplace_back(new Addr3::Less(token,$1,$3));
	else if($2.tipo == LESSEQUAL)
		semantico.code.emplace_back(new Addr3::LessEqual(token,$1,$3));
	else if($2.tipo == GREATER)
		semantico.code.emplace_back(new Addr3::Less(token,$3,$1));
	else if($2.tipo == GREATEREQUAL)
		semantico.code.emplace_back(new Addr3::LessEqual(token,$3,$1));
	else if($2.tipo == EQUAL)
		semantico.code.emplace_back(new Addr3::Equal(token,$1,$3));
	else if($2.tipo == NOTEQUAL)
		semantico.code.emplace_back(new Addr3::NotEqual(token,$1,$3));

	semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = " + tokenStrAtt($1) + " " + tokenStrAtt($2) + " " + tokenStrAtt($3));
	$$ = token;
} | additive_expression ;

relop: LESS | LESSEQUAL | GREATER | GREATEREQUAL | EQUAL | NOTEQUAL;

additive_expression: additive_expression addop term {
	pair<bool, Token> p = semantico.tempGen.obter();
	bool criado = p.first;
	Token token = p.second;

	if(criado){
		aloca_local(token,semantico);
		semantico.tabela.adicionar(tokenIdVal(token), INT, Simb::Nat::ARRAY, semantico.escopo, token, 1, true);
	}

	if($2.tipo == SUM)
		semantico.code.emplace_back(new Addr3::Adicao(token,$1,$3));	// token = $1 + $3
	else
		semantico.code.emplace_back(new Addr3::Subtracao(token,$1,$3));	// token = $1 - $3

	semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = " + tokenStrAtt($1) + " " + tokenStrAtt($2) + " " + tokenStrAtt($3));
	$$ = token;
} | term ;

addop: SUM | SUB ;

term: term mulop factor {
	pair<bool, Token> p = semantico.tempGen.obter();
	bool criado = p.first;
	Token token = p.second;

	if(criado){
		aloca_local(token,semantico);
		semantico.tabela.adicionar(tokenIdVal(token), INT, Simb::Nat::ARRAY, semantico.escopo, token, 1, true);
	}
	
	if($2.tipo == MUL)
		semantico.code.emplace_back(new Addr3::Multiplicacao(token,$1,$3) );	// token = $1 * $3
	else
		semantico.code.emplace_back(new Addr3::Divisao(token,$1,$3) );	// token = $1 / $3

	semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = " + tokenStrAtt($1) + " " + tokenStrAtt($2) + " " + tokenStrAtt($3));
	$$ = token;
} | factor ;

mulop: MUL | DIV ;

factor: LPAREN expression RPAREN {$$=$2;} | var | call | NUM ;

call: ID LPAREN Addr3_BeginCall args RPAREN { 
	semantico.tabela.verificar(tokenIdVal($1), Simb::Nat::FUNCAO);

	pair<bool, Token> p = semantico.tempGen.obter();
	bool criado = p.first;
	Token token = p.second;

	if(criado){
		aloca_local(token,semantico);
		semantico.tabela.adicionar(tokenIdVal(token), INT, Simb::Nat::ARRAY, semantico.escopo, token, 1, true);
	}

	Token token_original = semantico.tabela.obter_token(tokenIdVal($1));

	semantico.pseudoassembly.emplace_back(tokenStrAtt(token) + " = call " + tokenStrAtt(token_original));
	semantico.code.emplace_back(new Addr3::Call(token, token_original));

	$$ = token;
};

Addr3_BeginCall: %empty { 
	semantico.pseudoassembly.emplace_back("begin call");
	semantico.code.emplace_back(new Addr3::BeginCall);
} ;

args: arg_list | %empty ;

arg_list: arg_list COMMA addr3_param | addr3_param ;
	
addr3_param: expression { 
	semantico.pseudoassembly.emplace_back("Argumento " + tokenStrAtt($1));
	semantico.code.emplace_back(new Addr3::Param($1));
}

NUM: C_INT | C_FLOAT ;

%%

Zero::Zero() {
    token = tabsim.insert(C_INT);
	tabsim[token].insert(new IntVal("0"));
	tabsim[token].insert(new StrAtt("0"));
}
Token Zero::get() {return token;}

Token zeroIntVal() {
    Token token = tabsim.insert(C_INT);
    return token;
}

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
