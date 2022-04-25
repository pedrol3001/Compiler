#include "Sintatico.h"
#include <iostream>

using namespace std;

int main() {
	Lexico lexico("example");
	Sintatico sintatico(lexico);
	cout << "Status: " << sintatico.status << endl;
}
