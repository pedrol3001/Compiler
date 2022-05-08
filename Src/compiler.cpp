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
	
	// Lexico ===============================================
	// Input
	FILE* source = fopen(argv[0],"r");
	if(source==NULL) {
		cout << "Nao foi possivel abrir o arquivo '" << argv[0] << "'" << endl;
		return 0;
	}
	// Lexico
	Lexico lexico(source,stdout,false);	
	fclose(source);
	
	if(!lexico.good()) {
		cout << "Analise lexica FALHOU!" << endl;
		return 0;
	}
	cout << "Analise lexica bem sucedida!" << endl;
	// Sintatico ============================================
	Semantico semantico;
	Sintatico sintatico;
	sintatico.analisar(lexico,semantico);
	
	if(!sintatico.good()) {
		cout << "Analise sintatica FALHOU!" << endl;
		return 0;
	}	
	cout << "Analise sintatica bem sucedida!" << endl;
	// Semantico =============================================
	
	if(!semantico.good()) {
		cout << "Analise semantica FALHOU!\n" << endl;
		semantico.mostrar_analise();
		return 0;
	}	
	cout << "Analise semantica bem sucedida!\n" << endl;
	semantico.mostrar_analise();
	semantico.salvar_pseudoassembly();

	// Geracao de codigo =====================================
	// Output
	ofstream ofile(OUTPUT_NAME);
	if(!ofile.good()) {
		cout << "Falha ao criar arquivo de output!" << endl;
		return 0;
	}
	// Geracao	
	Gerador gerador;
	gerador.gerar(ofile,semantico.instrucoes());
	ofile.close();

	return 0;
}
