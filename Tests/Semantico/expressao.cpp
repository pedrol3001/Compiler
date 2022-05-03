#include "../test_format.cpp"
#include "../../Src/Semantico/Semantico.h"
#include "../../Src/Sintatico/Sintatico.h"
#include "../../Src/Lexico/Lexico.h"

#include "../../Src/TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <vector>
#include <cstdio>
#include <memory>

using namespace std;


bool test(Test& tester) {
	TabSim &ts = TabSim::getInstance();
	FILE * f = fopen("Resources/Semantico/expressao.txt","r");
	Lexico lexico(f,stdout,true);
	fclose(f);
	if(lexico.error()) {
		tester.error() << "Nao foi possivel abrir o arquivo!" << endl;
		return false;
	}
	tester.comment() << "Lexico ok!" << endl;
		
	Sintatico sintatico;
	sintatico.analisar(lexico);
	
	vector<string> ans = {"*","/","*","/","*","/","*","/"};
		
	tester.comment() << "Testando expressoes! (Para cada linha, todos tokens do bloco)" << endl;
	tester.normal() << sintatico.blocos().size() << endl;
	tester.comment() << endl << "..........................................." << endl;
	
	int i=0;
	for(shared_ptr<Bloco> bloco: sintatico.blocos()) {
		vector<Token> tokens = bloco->getTokens();
		for(Token token: tokens) {
			if(ts[token].has("StrAtt"))  {
				if(((StrAtt*)ts[token]["StrAtt"])->str != ans[i]) {
					
				}
				tester.normal() << ((StrAtt*)ts[token]["StrAtt"])->str << ' ';
			}
		}
		tester.comment() << endl << "..........................................." << endl;
		i++;
	}	

	return true;
}

int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
