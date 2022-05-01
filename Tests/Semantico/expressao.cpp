#include "../test_format.cpp"
#include "../../Src/Semantico/Semantico.h"
#include "../../Src/Sintatico/Sintatico.h"
#include "../../Src/Lexico/Lexico.h"

#include "../../Src/TabelaDeSimbolos/TabelaDeSimbolos.h"

#include <vector>

using namespace std;


bool test(Test& tester) {
	return false;

	TabSim ts = TabSim::getInstance();
	Lexico lexico("Resources/Semantico/expressao.txt");
	if(lexico.error()) {
		tester.error() << "Nao foi possivel abrir o arquivo!" << endl;
		return false;
	}
	tester.comment() << "Lexico ok!" << endl;
		
	Sintatico sintatico(lexico);
	
	tester.comment() << "Testando expressoes! (Para cada linha, todos tokens do bloco)" << endl;
	tester.normal() << sintatico.container.size() << endl;
	for(Bloco* bloco: sintatico.container) {
		vector<Token> tokens = bloco->getTokens();
		for(Token token: tokens) {
			if(ts[token].has("StrAtt"))
				tester.normal() << ((StrAtt*)ts[token]["StrAtt"])->str << ' ';
		}
		tester.comment() << "..........................................." << endl;
	}	

	return true;
}

int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
