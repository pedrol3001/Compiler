#include "../../Src/Sintatico/Sintatico.h"
#include <iostream>

#include "../test_format.cpp"

using namespace std;

bool test() {
	Lexico lexico("Resources/code6.txt");
	Sintatico sintatico(lexico);
	cout << "Status: " << sintatico.status << endl;
	return true;
}

int main(int argn, char *argv[]) {
	Test::execute(argv[1],test);
	return 0;
}
