#ifndef TabelaSemantica_h
#define TabelaSemantica_h

#include <string>
#include <utility>
#include <map>
#include <list>
#include <vector>
#include "../../Token/Token.h"
#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"

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
	std::vector<std::pair<std::pair<int, int>, std::string>> mensagens;
	std::vector<std::pair<Token, Token>> funcoes;
	std::vector<std::vector<Token>> fparams;

	TabelaSemantica();

	void adicionar(std::string nome, int bison_tipo, Simb::Nat natureza, int escopo, Token token, int tamanho, bool usado = false);	// int tipo => bison.h
	bool verificar(std::string nome, Simb::Nat natureza, Token token);
	void remover();
	void salvar_globais(std::list<std::string> pseudoassembly);

	bool existe(std::string nome);
	void marcar_usado(std::string nome);
	Token obter_token(std::string nome);
	Simb operator()(std::string nome,int escopo);
	bool existe(std::string nome,int escopo);
	Simb operator[](std::string nome);
	
	private:
	void updateTabSim(Token t);
	TabSim& tabsim;
};

struct TempGenerator{
	int temp_index = 0, max_index = 0;
	std::vector<Token> temps;
	Token gerar();
	std::pair<bool, Token> obter();
	void reset_index();
};

struct LabelGenerator{
	int label_index = 0;
	Token gerar();
};

#endif
