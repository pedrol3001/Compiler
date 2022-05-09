#include "Semantico.h"
#include "../Sintatico/Sintatico.h"
#include "../Util/Util.h"

#include <algorithm>
#include <iostream>
#include <cassert>
#include <fstream>

using namespace std;
using namespace Addr3;


Semantico::Semantico(): tabela(), escopo(tabela.escopo){
	
}

bool Semantico::analisar() {
	ok = this->tabela.erros_semantico == 0;	// ok eh o resultado da analise
	
	//gerarIntermediario();
	
	return ok;
}

struct ordenar_mensagens {
    bool operator()(const pair<pair<int, int>,string> &left, const pair<pair<int, int>,string> &right) {
        if(left.first == right.first)
			return left.second < right.second;
		return left.first < right.first;
    }
};


void Semantico::mostrar_analise() {

	sort(tabela.mensagens.begin(), tabela.mensagens.end(), ordenar_mensagens());


	for(auto &s : tabela.mensagens)
		std::cout << "Linha " << s.first.first+1 << ", coluna " << s.first.second+1 << ": " << s.second << endl;

	if(!tabela.mensagens.empty())
		std::cout << endl;

	std::cout << "Resultado do analisador semântico: ";
	std::cout << tabela.erros_semantico << " erro(s), " << tabela.avisos_semantico << " aviso(s)." << endl;
}

void Semantico::salvar_pseudoassembly() {
	std::ofstream out("TresEnderecos.txt");
	for(auto &s : pseudoassembly)
		out << s << endl;
    out.close();
	std::cout << "Arquivo TresEnderecos.txt criado com o código de 3 endereços." << endl;
}

void Semantico::gerarIntermediario() {
	assert(ok);
	
	code.clear();
	
	// Geracao de codigo intermediario
	// TODO: é um teste
}
#include <iostream>
std::list<std::shared_ptr<Addr3::Instrucao> > Semantico::instrucoes() {
	assert(ok);
	return code;
}

bool Semantico::good() {return ok;}
