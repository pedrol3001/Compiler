#ifndef TabelaSemantica_h
#define TabelaSemantica_h

#include <string>
#include <vector>
#include "../../Token/Token.h"

struct TabelaSemantica;
using TabSem = TabelaSemantica;


struct Simb {
	friend class TabelaSemantica;

	enum class Tipo{
		INT,VOID,ARRAY,
	};
	
	enum class Nat{
		VAR,FUNCAO,ARRAY,
	};
	
	Simb(std::string _nome, Tipo _tipo, int _escopo, Nat _natureza, long long int _tamanho = 1);
	
	std::string nome;
	Tipo tipo; // 0 = int, 1 = void
	int escopo;	// Nivel do escopo
	Nat natureza; // 0 = var, 1 = funcao, 2 = array	
	long long int tamanho = 1;
	
	bool usado = false;
	
	long long int offset();
	
	~Simb();	// Atualiza max_distance_to_base
	
	private:
		static long long int max_distance_to_base;
		long long int distance_to_base;	// distancia ate o valor minimo do ponteiro 
	
	
};

struct LocalSimb: public Simb {

};

struct GlobalSimb: public Simb {

};
	
struct TabelaSemantica {	
	int erros_semantico = 0, escopo = 0;
	std::vector<Simb> locais;
	std::vector<Simb> globais;

	TabelaSemantica();

	// int tipo => bison.h
	void adicionar(std::string nome, int bison_tipo, Simb::Nat natureza, int escopo, int tamanho);
	int existe(std::string nome);
	long long int offset(std::string nome);
	bool verificar(std::string nome, Simb::Tipo tipo);
	void desalocar();
	void mostrar_globais();
};

struct TempGenerator{
	int temp_index = 0;
	
	Token gerar();
	void reset_index();
};

#endif
