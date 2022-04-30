#include "../../Src/Lexico/Lexico.h"

#include <iostream>
#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {
	TabSim &ts = TabSim::getInstance();
	Lexico lexico("Resources/code6.txt");
	tester.normal() << lexico.size() << endl;
	
	tester.comment() << "\nTeste do sem operador >>" << endl;
	for(Token token: lexico) {
		StrAtt * sa = (StrAtt*)ts[token.id()]["StrAtt"];
		tester.normal() << "Token: " << sa->str << endl;
		
		NameAtt * na = (NameAtt*)ts[token.id()]["NameAtt"];
		tester.normal() << "Tipo: " << na->str << endl;
		
		tester.normal() << "Enum: " << token() << "\tn: " << token.id() << endl;
		
		tester.comment() << "===================================" << endl;
	}
	
	tester.comment() << "\nTeste do operador >>" << endl;
	Token token;
	while(lexico >> token) {
		StrAtt * sa = (StrAtt*)ts[token.id()]["StrAtt"];
		tester.normal() << "Token: " << sa->str << endl;
		
		NameAtt * na = (NameAtt*)ts[token.id()]["NameAtt"];
		tester.normal() << "Tipo: " << na->str << endl;
		
		tester.normal() << "Enum: " << token() << "\tn: " << token.id() << endl;
		
		tester.comment() << "===================================" << endl;
	}
	return true;
}


int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
