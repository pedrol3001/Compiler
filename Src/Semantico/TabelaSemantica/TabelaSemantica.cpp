#include "TabelaSemantica.h"
#include "../../Tipos.h"

#include <iostream>
#include <cassert>

using namespace std;

//Simbolo ==============================================================================================================

Simb::Simb(std::string _nome, Tipo _tipo, int _escopo, Nat _natureza, Token _token, long long int _tamanho) :
	nome(_nome), tipo(_tipo), escopo(_escopo), natureza(_natureza), token(_token), tamanho(_tamanho) {}

// Tabela Semantica ====================================================================================================

TabelaSemantica::TabelaSemantica(): tabsim(TabSim::getInstance()) {}

void TabelaSemantica::updateTabSim(Token t) {
	if(tabsim[t].has("IsTemp")) {
		if(!tabsim[t].has("VarLocal"))
			tabsim[t].insert((Atributo*) new VarLocal(1));
		return;
	}

	assert(tabsim[t].has("IdVal"));
	string idval = ((IdVal*)tabsim[t]["IdVal"])->val;
	if(existe(idval)) {
		Simb simb = (*this)[idval];
		cout << simb.escopo << ": " << idval << endl;
		if(simb.escopo==GLOBAL) {
			if(!tabsim[t].has("VarGlobal"))
				tabsim[t].insert((Atributo*) new VarGlobal(simb.tamanho));
		} else {
			if(!tabsim[t].has("VarLocal"))
				tabsim[t].insert((Atributo*) new VarLocal(simb.tamanho));
		}
	}
}

void TabelaSemantica::adicionar(std::string nome, int bison_tipo, Simb::Nat natureza, int escopo, Token token, int tamanho, bool usado){
	if(existe(nome) && (*this)[nome].escopo < escopo){
		std::cout << "Aviso: variável \"" << nome << "\" sendo substituída por variável local." << std::endl;
		avisos_semantico++;
	}else if(existe(nome) && (*this)[nome].escopo == escopo){
		std::cout << "Erro: variável \"" << nome << "\" sendo redeclarada." << std::endl;
		erros_semantico++;
		return;
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
		return;
	}

	variaveis[nome].push_back(Simb(nome, tipo, escopo, natureza, token, tamanho));
	if(usado)
		variaveis[nome].back().usado = true;
	updateTabSim(token);
	
	//cout << "aloca " << (*this)[nome].nome << " " << (*this)[nome].tamanho << "\n";
}

bool TabelaSemantica::existe(string nome){
	return variaveis.count(nome)>0 && !variaveis[nome].empty();
}

void TabelaSemantica::marcar_usado(string nome){
	variaveis[nome].back().usado = true;
}

Token TabelaSemantica::obter_token(string nome){
	return variaveis[nome].back().token;
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
			//cout << "desaloca " << lista.back().nome << " " << lista.back().tamanho << std::endl;
			if(lista.back().usado == false){
				cout << "Aviso: variável \"" << lista.back().nome << "\" não usada." << endl;
				avisos_semantico++;
			}
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
	cout << "=============================================\n";
}
	
Token TempGenerator::gerar(){
	TabSim &tabsim = TabSim::getInstance();
	Token token = tabsim.insert(ID);
	string name = "$t" + to_string(temp_index);
	tabsim[token].insert((Atributo*)(new IdVal(name)));
	tabsim[token].insert((Atributo*)(new StrAtt(name)));
	tabsim[token].insert((Atributo*)(new IsTemp));
	return token;
}

pair<bool, Token> TempGenerator::obter(){
	//cout << "Token requisitado. temp_index = " << temp_index << ", max_index = " << max_index << endl;
	if(temp_index == max_index){
		Token token = gerar();
		max_index++, temp_index++;
		temps.push_back(token);
		return {true, token};
	}
	return {false, temps[temp_index++]};
	
}

void TempGenerator::reset_index(){temp_index = 0;}

Token LabelGenerator::gerar(){
	TabSim &tabsim = TabSim::getInstance();
	Token token = tabsim.insert(ID);
	string name = "$L" + to_string(label_index++);
	tabsim[token].insert((Atributo*)(new IdVal(name)));
	tabsim[token].insert((Atributo*)(new StrAtt(name)));
	tabsim[token].insert((Atributo*)(new IsTemp));
	tabsim[token].insert((Atributo*)new LabelVal);
	return token;
}
