#include "../../Src/Lexico/Lexico.h"

#include <iostream>
#include "../test_format.cpp"

using namespace std;

bool test(Test& tester) {

	TabSim &ts = TabSim::getInstance();
	Lexico lexico("Resources/lexico.txt");
	tester.normal() << lexico.size() << endl;
	
	tester.comment() << "\nTeste sem operador >>" << endl;
	for(Token token: lexico.container) {
		tester.normal() << "Enum: " << token() << "\tn: " << token.id() << endl;
		
		// Str Arr
		if(!ts[token].has("StrAtt"))
			tester.normal() << "Token sem atributo StrArr" << endl;
		else {
			StrAtt * sa = (StrAtt*)(ts[token]["StrAtt"]);
			tester.normal() << "Token: " << sa->str << endl;
		}
		
		if(!ts[token].has("NameAtt"))
			tester.normal() << "Token sem atributo NameAtt" << endl;
		else {
			NameAtt * na = (NameAtt*)ts[token]["NameAtt"];
			tester.normal() << "Tipo: " << na->str << endl;
		}
		
		tester.comment() << "===================================" << endl;
	}
	
	tester.comment() << "\nTeste do operador >>" << endl;
	Token token;
	while(lexico >> token) {
		tester.normal() << "Enum: " << token() << "\tn: " << token.id() << endl;
		
		// Str Arr
		if(!ts[token].has("StrAtt"))
			tester.normal() << "Token sem atributo StrArr" << endl;
		else {
			StrAtt * sa = (StrAtt*)ts[token]["StrAtt"];
			tester.normal() << "Token: " << sa->str << endl;
		}
		
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
