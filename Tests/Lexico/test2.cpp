#include "../../Src/Lexico/Lexico.h"

#include <iostream>
#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {

	TabSim &ts = TabSim::getInstance();
	FILE* f = fopen("Resources/teste_comentario.txt","r");
	Lexico lexico(f,stdout,true);
	fclose(f);
	if(lexico.error()) {
		tester.error() << "Nao foi possivel abrir o arquivo!" << endl;
		return false;
	}
	
	tester.normal() << lexico.size() << endl;
	
	vector<string> ans = {"/* /**/","*","/" ,"," ,"/* /* */","*","/"};
	
	int i=0;
	Token token;
	while(lexico >> token) {
		if(i>=ans.size()) {
			tester.error() << "Numero incorreto de tokens encontrado!" << endl;
			tester.log() << "Esperavam-se " << ans.size() <<  " tokens." << endl;
			return false;
		}
	
		tester.normal() << "Enum: " << token() << "\tn: " << token.id() << endl;
		
		// Str Arr
		if(!ts[token].has("StrAtt"))
			tester.normal() << "Token sem atributo StrArr" << endl;
		else {
			StrAtt * sa = (StrAtt*)(ts[token]["StrAtt"]);
			
			if(sa->str != ans[i]) {
				tester.error() << "Token '" << sa->str << "' incorreto!" << endl;
				tester.log() << "Esperava-se '" << ans[i] << "'." << endl;
				return false;
			}
			
			tester.normal() << "Token: " << sa->str << endl;
		}
		
		// Name Att
		if(!ts[token].has("NameAtt"))
			tester.normal() << "Token sem atributo NameAtt" << endl;
		else {
			NameAtt * na = (NameAtt*)ts[token]["NameAtt"];
			tester.normal() << "Tipo: " << na->str << endl;
		}
		
		tester.comment() << "===================================" << endl;
	}
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
