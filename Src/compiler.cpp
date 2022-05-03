#include "Lexico/Lexico.h"
#include "Sintatico/Sintatico.h"
#include "Semantico/Semantico.h"
#include "Gerador/Gerador.h"

#include <fstream>
#include <iostream>
#include <ostream>

using namespace std;

#define OUTPUT_NAME "programa.tm"

int main(int argn, char *argv[]) {
	argn--; argv++;
	
	// Verificar argumentos =================================
	if(argn != 1) {
		cout << "Formato de argumento incorreto!" << endl;
		cout << "./compiler [programa]" << endl;
		return 0;
	}
	// Verificar input e output =============================
	// Input
	FILE* source = fopen(argv[0],"r");
	if(source==NULL) {
		cout << "Nao foi possivel abrir o arquivo '" << argv[0] << "'" << endl;
		return 0;
	}
	// Output
	ofstream ofile(OUTPUT_NAME);
	if(!ofile.good()) {
		cout << "Falha ao criar arquivo de output!" << endl;
		return 0;
	}
	
	// Lexico ===============================================
	Lexico lexico(source,stdout);	
	if(!lexico.good()) {
		cout << "\nAnalise lexica FALHOU!" << endl;
		return 0;
	}
	cout << "\nAnalise lexica bem sucedida!" << endl;
	// Sintatico ============================================
	Sintatico sintatico;
	sintatico.analisar(lexico);
	
	if(!sintatico.good()) {
		cout << "\nAnalise sintatica FALHOU!" << endl;
		return 0;
	}	
	cout << "\nAnalise sintatica bem sucedida!" << endl;
	// Semantico =============================================
	Semantico semantico;
	semantico.analisar(sintatico.blocos());
	
	if(!semantico.good()) {
		cout << "\nAnalise semantica FALHOU!" << endl;
		return 0;
	}	
	cout << "\nAnalise semantica bem sucedida!" << endl;
	// Geracao de codigo
	
	Gerador gerador;
	gerador.gerar(ofile,semantico.instrucoes());
	
	ofile.close();

	return 0;
}
