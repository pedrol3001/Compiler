#include "TabelaSemantica.h"
#include "../../Tipos.h"

#include <iostream>
#include <cassert>

using namespace std;

long long int Simb::max_distance_to_base=0;

Simb::Simb(std::string _nome, Tipo _tipo, int _escopo, Nat _natureza, long long int _tamanho): nome(_nome), tipo(_tipo), escopo(_escopo), natureza(_natureza), tamanho(_tamanho) {
	distance_to_base = (Simb::max_distance_to_base += tamanho);
}
long long int Simb::offset() {
	return Simb::max_distance_to_base - distance_to_base + 1;
}

Simb::~Simb() {
	//assert(Simb::max_distance_to_base == distance_to_base);	// Verificar se a remocao faz sentido (desalocar primeiro o que esta no topo da pilha)
	Simb::max_distance_to_base -= tamanho;
}

TabelaSemantica::TabelaSemantica(){}

void TabelaSemantica::adicionar(string nome, int bison_tipo, Simb::Nat natureza, int escopo, int tamanho){
	int st = this->existe(nome);
	if(st != -1 && locais[st].escopo < escopo){
		std::cout << "Aviso: variável \"" << nome << "\" sendo substituída por variável local." << std::endl;
	}else if(st != -1 && locais[st].escopo == escopo){
		std::cout << "Erro: variável \"" << nome << "\" sendo redeclarada." << std::endl;
	}

	Simb::Tipo tipo;
	switch(bison_tipo) {
		case INT: tipo =  Simb::Tipo::INT; 
			break;
		case VOID: tipo = Simb::Tipo::VOID;
			break;
	
	}
		
	Simb s(nome, tipo, escopo, natureza, tamanho);

	if(natureza == Simb::Nat::VAR && tipo == Simb::Tipo::VOID){ // variavel com tipo void
		std::cout << "Erro: variável \"" << nome << "\" não pode ser do tipo void." << std::endl;
		erros_semantico++;
	}

	this->locais.push_back(s);
	if(s.escopo == 0)
		this->globais.push_back(s);
	else
		cout << "aloca " << s.nome << " " << s.tamanho << "\n";
}
int TabelaSemantica::existe(string nome){
	for(auto it = locais.rbegin(); it != locais.rend(); it++)
		if(it->nome == nome) return std::distance(it, locais.rend());
	return -1;
}
long long int TabelaSemantica::offset(string nome){
	return existe(nome) - globais.size();
}
bool TabelaSemantica::verificar(string nome, Simb::Tipo tipo){
	for(auto it = locais.rbegin(); it != locais.rend(); it++){
		if(it->nome == nome){
			if(it->natureza != Simb::Nat::FUNCAO && tipo == Simb::Tipo::VOID){ // não-função sendo usada como função
				std::cout << "Erro: variável \"" << nome << "\" não pode ser usada como uma função." << std::endl;
				erros_semantico++;
				return false;
			}
			if(it->natureza == Simb::Nat::VAR && tipo == Simb::Tipo::ARRAY){ // variável normal sendo usada como array
				std::cout << "Erro: variável \"" << nome << "\" não pode ser usada como um array." << std::endl;
				erros_semantico++;
				return false;
			}

			it->usado = true;
			return true;
		};
	}
	std::cout << "Erro: variável \"" << nome << "\" não declarada." << std::endl;
	erros_semantico++;
	return false;
}
void TabelaSemantica::desalocar(){
	while(!locais.empty() && locais.back().escopo > escopo){
		cout << "desaloca " << locais.back().nome << " " << locais.back().tamanho << std::endl;
		locais.pop_back();
	}
}
void TabelaSemantica::mostrar_globais(){
	cout << "=============================================\n";
	for(auto it = globais.begin(); it != globais.end(); it++)
		cout << "global: " << it->nome << " " << it->tamanho << std::endl; 
}

	
Token TempGenerator::gerar(){
	TabSim &tabsim = TabSim::getInstance();
	Token token = tabsim.insert(TEMP);
	string name = "$t" + to_string(temp_index++);
	tabsim[token].insert((Atributo*)(new IdVal(name)));
	tabsim[token].insert((Atributo*)(new StrAtt(name)));
	return token;
}
void TempGenerator::reset_index(){
	temp_index = 0;
}

