#include "TabelaSemantica.h"
#include "../../Tipos.h"

#include <iostream>
#include <cassert>
#include <string>

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
		mensagens.push_back("Aviso: variável \"" + nome + "\" sendo substituída por variável local.");
		avisos_semantico++;
	}else if(existe(nome) && (*this)[nome].escopo == escopo){
		mensagens.push_back("Erro: variável \"" + nome + "\" sendo redeclarada.");
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
		mensagens.push_back("Erro: variável \"" + nome + "\" declarada como void.");
		erros_semantico++;
		return;
	}

	variaveis[nome].push_back(Simb(nome, tipo, escopo, natureza, token, tamanho));
	if(usado)
		variaveis[nome].back().usado = true;
	updateTabSim(token);
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
		mensagens.push_back("Erro: variável \"" + nome + "\" não declarada.");
		erros_semantico++;
		return false;
	}

	static map<Simb::Nat,string> str = {
		{Simb::Nat::FUNCAO,"função"},
		{Simb::Nat::VAR,"variável"},
		{Simb::Nat::ARRAY,"array"}
	};
	
	if((*this)[nome].natureza != natureza) {
		mensagens.push_back( "Erro: " + str[(*this)[nome].natureza] + " \"" + nome + "\" usado(a) como " + str[natureza] + ".");
		erros_semantico++;
		return false;
	}
	return true;
}

void TabelaSemantica::remover(){
	for(pair<const string,list<Simb> >& p: variaveis) {
		list<Simb>& lista = p.second;
		while(!lista.empty() && lista.back().escopo > escopo) {
			if(lista.back().usado == false){
				mensagens.push_back("Aviso: variável \"" + lista.back().nome + "\" não usada.");
				avisos_semantico++;
			}
			lista.pop_back();
		}
	}
}

void TabelaSemantica::salvar_globais(list<string> pseudoassembly){
	for(pair<string,list<Simb> > p: variaveis) {
		list<Simb> &lista = p.second;
		if(lista.empty()) continue;
		for(Simb s: lista)
			if(s.escopo==0){
				if(s.tamanho == 1)
					pseudoassembly.emplace_back("global: " + p.first);
				else
					pseudoassembly.emplace_back("global: " + p.first + " [" + to_string(s.tamanho) + "]");
			}
	}
	pseudoassembly.emplace_back("=============================================");
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
