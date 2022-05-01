#ifndef Tabela_h
#define Tabela_h

#include "../Token/Token.h"

#include <vector>
#include <map>
#include <string>

// Unidade de acesso
struct Token;		
using Tok = Token;
// Atributo do token
struct Atributo;	
using Att = Atributo;
// Conjunto de atributos de um token
struct Simbolo;	
using Sim = Simbolo;
// Tabela de simbolos
struct TabelaDeSimbolos;
using TabSim = TabelaDeSimbolos;

/* A tabela de símbolos é um vetor de conjuntos de atributos, com nome em forma de string*/
struct TabelaDeSimbolos {
	std::vector<Simbolo> container; 	// Simbolos
	static TabelaDeSimbolos& getInstance();	// Caso seja usada como Singleton
	Token insert(int tipo);			// Insere token, dado o tipo
	long long unsigned int size();
	Simbolo& operator[](Token& token);
	~TabelaDeSimbolos();
	private:
		TabelaDeSimbolos();
};

/* Simbolo eh um conjunto de atributos, que podem ser inseridos pelos analisadores lexico, sintatico e semantico
	Exemplo de atributos: valor, tipo (int ou long int?), constante (nunca variou apos inicializacao), etc.  
*/
struct Simbolo {
	std::map<std::string,Atributo*> container;	// Atributos
	Atributo* operator[](std::string key);
	bool has(std::string key);
	void insert(Atributo* att);	
	void free();
};

struct Atributo{
	std::string nome;
	Atributo(std::string _nome);
	virtual ~Atributo();
};



#endif
