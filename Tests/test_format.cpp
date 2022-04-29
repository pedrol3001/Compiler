#include <iostream>
#include <string>

using namespace std;

class Test {
	static void begin(string agv);
	static void fail(string agv);
	static void ok(string agv);
	static void end(string agv);	
	public:
		static void execute(string argv, bool (*func)()) {
			begin(argv);
			if(func())
				ok(argv);
			else
				fail(argv);
			end(argv);
		}
};

void Test::begin(string argv) {
	cout << "===========================================" << endl;
	cout << "Inicio do teste: " << argv << endl;
	cout << "===========================================" << endl;
}

void Test::fail(string argv) {
	cout << "-------------------------------------------" << endl;
	cout << "!! Falha no teste: " << argv << endl;
	cout << "-------------------------------------------" << endl;
}

void Test::ok(string argv) {
	cout << "-------------------------------------------" << endl;
	cout << "Sucesso no teste: " << argv << endl;
	cout << "-------------------------------------------" << endl;
}

void Test::end(string argv) {
	cout << "===========================================" << endl;
	cout << "Fim do teste: " << argv << endl;
	cout << "===========================================" << endl;
}
