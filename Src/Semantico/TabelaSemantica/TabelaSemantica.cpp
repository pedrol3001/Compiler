#include "TabelaSemantica.h"
#include "../../Tipos.h"

#include <iostream>

using namespace std;

TabelaSemantica::TabelaSemantica(){
	vars.push_back(Simb{"input", Simb::Tipo::VOID, 0, Simb::Nat::FUNCAO});
	vars.push_back(Simb{"output", Simb::Tipo::VOID, 0, Simb::Nat::FUNCAO});
}

void TabelaSemantica::adicionar(string nome, int bison_tipo, Simb::Nat natureza, int escopo, string tamanho){
	int st = this->existe(nome);
	if(st != -1 && vars[st].escopo < escopo){
		std::cout << "Aviso: variável \"" << nome << "\" sendo substituída por variável local." << std::endl;
	}else if(st != -1 && vars[st].escopo == escopo){
		std::cout << "Erro: variável \"" << nome << "\" sendo redeclarada." << std::endl;
	}

	Simb::Tipo tipo;
	if(bison_tipo == INT)
		tipo = Simb::Tipo::INT; // int
	else if(bison_tipo == VOID)
		tipo = Simb::Tipo::VOID; // void
		
	Simb s{nome, tipo, escopo, natureza, tamanho};

	if(natureza == Simb::Nat::VAR && tipo == Simb::Tipo::VOID){ // variavel com tipo void
		std::cout << "Erro: variável \"" << nome << "\" não pode ser do tipo void." << std::endl;
		erros_semantico++;
	}

	this->vars.push_back(s);
	if(s.escopo == 0)
		this->globais.push_back(s);
	else
		cout << "aloca " << s.nome << " " << s.tamanho << "\n";
}
int TabelaSemantica::existe(string nome){
	for(auto it = vars.rbegin(); it != vars.rend(); it++)
		if(it->nome == nome) return distance(it, vars.rend());
	return -1;
}
int TabelaSemantica::offset(string nome){
	return existe(nome) - globais.size();
}
bool TabelaSemantica::verificar(string nome, Simb::Tipo tipo){
	for(auto it = vars.rbegin(); it != vars.rend(); it++){
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
	while(!vars.empty() && vars.back().escopo > escopo){
		cout << "desaloca " << vars.back().nome << " " << vars.back().tamanho << std::endl;
		vars.pop_back();
	}
}
void TabelaSemantica::mostrar_globais(){
	cout << "=============================================\n";
	for(auto it = globais.begin(); it != globais.end(); it++)
		cout << "global: " << it->nome << " " << it->tamanho << std::endl; 
}

	
string Suporte::obter_temporario(){
	return "t" + std::to_string(temp_index++);
}
void Suporte::reset_index(){
	temp_index = 0;
}

