#ifndef TabelaSemantica_h
#define TabelaSemantica_h

#include <string>
#include <vector>

struct TabelaSemantica;
using TabSem = TabelaSemantica;


struct Simb {
	enum class Tipo{
		INT,VOID,ARRAY,
	};
	
	enum class Nat{
		VAR,FUNCAO,ARRAY,
	};
	
	std::string nome;
	Tipo tipo; // 0 = int, 1 = void
	int escopo;
	Nat natureza; // 0 = var, 1 = funcao, 2 = array
	std::string tamanho = "1";
	bool usado = false;
	
};
	
struct TabelaSemantica {	
	int erros_semantico = 0, escopo = 0;
	std::vector<Simb> vars;
	std::vector<Simb> globais;

	TabelaSemantica();

	// int tipo => bison.h
	void adicionar(std::string nome, int bison_tipo, Simb::Nat natureza, int escopo, std::string tamanho);
	int existe(std::string nome);
	int offset(std::string nome);
	bool verificar(std::string nome, Simb::Tipo tipo);
	void desalocar();
	void mostrar_globais();
};

struct Suporte{
	int temp_index = 0;
	
	std::string obter_temporario();
	void reset_index();
};

#endif
