#include "TabelaSemantica.h"
#include "../../Tipos.h"

#include <iostream>
#include <cassert>

using namespace std;

//Simbolo ==============================================================================================================

Simb::Simb(std::string _nome, Tipo _tipo, int _escopo, Nat _natureza, long long int _tamanho): 
	nome(_nome), tipo(_tipo), escopo(_escopo), natureza(_natureza), tamanho(_tamanho) {}

// Tabela Semantica ====================================================================================================

TabelaSemantica::TabelaSemantica(){}

void TabelaSemantica::adicionar(string nome, int bison_tipo, Simb::Nat natureza, int escopo, int tamanho){
	if(existe(nome) && (*this)[nome].escopo < escopo){
		std::cout << "Aviso: variável \"" << nome << "\" sendo substituída por variável local." << std::endl;
	}else if(existe(nome) && (*this)[nome].escopo == escopo){
		std::cout << "Erro: variável \"" << nome << "\" sendo redeclarada." << std::endl;
	}

	Simb::Tipo tipo;
	// Converter do tipo bison para o tipo Simb::Tipo
	switch(bison_tipo) {
		case INT: tipo =  Simb::Tipo::INT; 
			break;
		case VOID: tipo = Simb::Tipo::VOID;
			break;
	}

	if(natureza == Simb::Nat::VAR && tipo == Simb::Tipo::VOID){ // variavel com tipo void
		std::cout << "Erro: variável \"" << nome << "\" não pode ser do tipo void." << std::endl;
		erros_semantico++;
	}

	variaveis[nome].push_back(Simb(nome, tipo, escopo, natureza, tamanho));
	cout << "aloca " << (*this)[nome].nome << " " << (*this)[nome].tamanho << "\n";
}

bool TabelaSemantica::existe(string nome){
	return variaveis.count(nome)>0 && !variaveis[nome].empty();
}
Simb TabelaSemantica::operator[](string nome) {
	assert(existe(nome));
	return variaveis[nome].back();
}

bool TabelaSemantica::verificar(string nome, Simb::Nat natureza){
	if(!existe(nome)) {
		cout << "Erro: variável \"" << nome << "\" não declarada." << endl;
		erros_semantico++;
		return false;
	}

	static map<Simb::Nat,string> str = {
		{Simb::Nat::FUNCAO,"função"},
		{Simb::Nat::VAR,"variável"},
		{Simb::Nat::ARRAY,"array"}
	};
	if((*this)[nome].natureza != natureza) {
		cout << "Erro: " << str[(*this)[nome].natureza] << " \"" << nome << "\" ";
		cout<<  "usado(a) como " << str[natureza] << "." << endl;
		erros_semantico++;
		return false;
	}
	return true;
}
void TabelaSemantica::remover(){
	for(pair<const string,list<Simb> >& p: variaveis) {
		list<Simb>& lista = p.second;
		while(!lista.empty() && lista.back().escopo > escopo) {
			cout << "desaloca " << lista.back().nome << " " << lista.back().tamanho << std::endl;
			lista.pop_back();
		}
	}
}
void TabelaSemantica::mostrar_globais(){
	cout << "=============================================\n";
	for(pair<string,list<Simb> > p: variaveis) {
		list<Simb> &lista = p.second;
		if(lista.empty()) continue;
		for(Simb s: lista)
			if(s.escopo==0)
				cout << "global: " << p.first << " " << s.tamanho << std::endl; 
	}
}

	
Token TempGenerator::gerar(){
	TabSim &tabsim = TabSim::getInstance();
	Token token = tabsim.insert(ID);
	string name = "$t" + to_string(temp_index++);
	tabsim[token].insert((Atributo*)(new IdVal(name)));
	tabsim[token].insert((Atributo*)(new StrAtt(name)));
	tabsim[token].insert((Atributo*)(new IsTemp));
	return token;
}
void TempGenerator::reset_index(){temp_index = 0;}

