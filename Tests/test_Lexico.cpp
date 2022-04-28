#include "Lexico.h"

#include <iostream>

using namespace std;

int main() {
	TabSim &ts = TabSim::getInstance();
	Lexico lexico("example");
	
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
	
	cout << "Fim do teste da classe Lexico." << endl;

	return 0;
}
