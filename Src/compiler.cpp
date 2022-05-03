#include "Lexico/Lexico.h"
#include "Sintatico/Sintatico.h"
#include "Semantico/Semantico.h"
#include "Gerador/Gerador.h"

#include <iostream>

using namespace std;

int main(int argn, char *argv[]) {
	argn--; argv++;
	
	if(argn != 1) {
		cout << "Formato de argumento incorreto!" << endl;
		cout << "./compiler [programa]" << endl;
		return 0;
	}
	
	Lexico lexico(argv[0]);	// Montar o lexico
	if(lexico.good()) {
		cout << "\nAnalise lexica bem sucedida!" << endl;
		Sintatico sintatico(lexico);
		
		if(sintatico.good()) {
			cout << "\nAnalise sintatica bem sucedida!" << endl;
		}	
	}
	

	return 0;
}
