#include "../../Src/Lexico/Lexico.h"

#include <iostream>
#include "../test_format.cpp"

using namespace std;

bool test() {
	TabSim &ts = TabSim::getInstance();
	Lexico lexico("Resources/code6.txt");
	cout << lexico.size() << endl;
	
	cout << "\nTeste do sem operador >>" << endl;
	for(Token token: lexico) {
		StrAtt * sa = (StrAtt*)ts[token.id()]["StrAtt"];
		cout << "Token: " << sa->str << endl;
		
		NameAtt * na = (NameAtt*)ts[token.id()]["NameAtt"];
		cout << "Tipo: " << na->str << endl;
		
		cout << "Enum: " << token() << "\tn: " << token.id() << endl;
	}
	
	cout << "\nTeste do operador >>" << endl;
	Token token;
	while(lexico >> token) {
		StrAtt * sa = (StrAtt*)ts[token.id()]["StrAtt"];
		cout << "Token: " << sa->str << endl;
		
		NameAtt * na = (NameAtt*)ts[token.id()]["NameAtt"];
		cout << "Tipo: " << na->str << endl;
		
		cout << "Enum: " << token() << "\tn: " << token.id() << endl;
	}
	return true;
}


int main(int argn, char *argv[]) {
	Test::execute(argv[1],test);
	return 0;
}
