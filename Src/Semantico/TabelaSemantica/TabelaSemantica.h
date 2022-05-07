#ifndef TabelaSemantica_h
#define TabelaSemantica_h

#include <string>
#include <utility>
#include <map>
#include <list>
#include "../../Token/Token.h"

struct TabelaSemantica;
using TabSem = TabelaSemantica;


struct Simb {
	friend class TabelaSemantica;

	enum class Tipo{
		INT,VOID,
	};
	
	enum class Nat{
		VAR,FUNCAO,ARRAY,
	};
	
	Simb(std::string _nome, Tipo _tipo, int _escopo, Nat _natureza, Token token, long long int _tamanho = 1);
	
	std::string nome;
	Tipo tipo; // 0 = int, 1 = void
	int escopo;	// Nivel do escopo
	Nat natureza; // 0 = var, 1 = funcao, 2 = array	
	long long int tamanho = 1;
	Token token;
	
	bool usado = false;
};

enum {GLOBAL=0};

struct TabelaSemantica {
	std::map<std::string,std::list<Simb> > variaveis;

	int erros_semantico = 0, avisos_semantico = 0, escopo = GLOBAL;

	TabelaSemantica();

	void adicionar(std::string nome, int bison_tipo, Simb::Nat natureza, int escopo, int tamanho, Token token);	// int tipo => bison.h
	bool verificar(std::string nome, Simb::Nat natureza);
	void remover();
	void mostrar_globais();

	bool existe(std::string nome);
	void marcar_usado(std::string nome);
	Token obter_token(std::string nome);
	Simb operator()(std::string nome,int escopo);
	bool existe(std::string nome,int escopo);
	Simb operator[](std::string nome);
};

struct TempGenerator{
	int temp_index = 0;
	Token gerar();
	void reset_index();
};

struct LabelGenerator{
	int label_index = 0;
	Token gerar();
};

#endif
