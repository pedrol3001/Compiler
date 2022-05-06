#ifndef TabelaSemantica_h
#define TabelaSemantica_h

#include <string>
#include <vector>

struct TabelaSemantica;
using TabSem = TabelaSemantica;

struct Simb {
	std::string nome;
	int tipo; // 0 = int, 1 = void
	int escopo;
	int natureza; // 0 = var, 1 = funcao, 2 = array
	std::string tamanho = "1";
	bool usado = false;
};
	
struct TabelaSemantica {	
	int erros_semantico = 0, escopo = 0;
	std::vector<Simb> vars;
	std::vector<Simb> globais;

	TabelaSemantica();

	void adicionar(std::string nome, int tipo, int natureza, int escopo, std::string tamanho);
	int existe(std::string nome);
	int offset(std::string nome);
	bool verificar(std::string nome, int tipo);
	void desalocar();
	void mostrar_globais();
};

struct Suporte{
	int temp_index = 0;
	
	std::string obter_temporario();
	void reset_index();
};

#endif
