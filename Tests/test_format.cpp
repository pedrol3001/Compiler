// Windows
#ifdef _WIN32
	#define RESET_COLOR 	""

	#define ERR_COLOR 	""
	#define ERR_BARS_COLOR	""

	#define OK_COLOR 	""
	#define OK_BARS_COLOR 	""

	#define LOG_COLOR  	""
	#define LOG_BARS_COLOR	""
	
// Linux
#elif __linux__
	#define RESET_COLOR 	"\x1B[0m"

	#define ERR_COLOR 	"\x1B[31m"
	#define ERR_BARS_COLOR	"\x1B[91m"

	#define OK_COLOR 	"\x1B[32m"
	#define OK_BARS_COLOR 	"\x1B[92m"

	#define LOG_COLOR  	"\x1B[33m"
	#define LOG_BARS_COLOR	"\x1B[93m"

// MacOS? e outros
#else 
	#define RESET_COLOR 	""

	#define ERR_COLOR 	""
	#define ERR_BARS_COLOR	""

	#define OK_COLOR 	""
	#define OK_BARS_COLOR 	""

	#define LOG_COLOR  	""
	#define LOG_BARS_COLOR	""
	
#endif
	
#include <string>
#include <iostream>

using namespace std;

class Test {
	static void begin(string agv);
	static void fail(string agv);
	static void ok(string agv);
	static void end(string agv);	
	public:
		static void execute(string argv, bool (*func)()) {
			cout << endl;
			begin(argv);
			if(func())
				ok(argv);
			else
				fail(argv);
			end(argv);
			cout << RESET_COLOR << endl;	// Resetar cor
		}
};

void Test::begin(string argv) {
	cout << LOG_BARS_COLOR;
	cout << "===========================================" << endl;
	cout << LOG_COLOR;
	cout << "Inicio do teste: " << argv << endl;
	cout << LOG_BARS_COLOR;
	cout << "===========================================" << endl;
	cout << RESET_COLOR;
}

void Test::fail(string argv) {
	cout << ERR_BARS_COLOR;
	cout << "-------------------------------------------" << endl;
	cout << ERR_COLOR;
	cout << "Falha no teste: " << argv << endl;
	cout << ERR_BARS_COLOR;
	cout << "-------------------------------------------" << endl;
	cout << RESET_COLOR;
}

void Test::ok(string argv) {
	cout << OK_BARS_COLOR;
	cout << "-------------------------------------------" << endl;
	cout << OK_COLOR;
	cout << "Sucesso no teste: " << argv << endl;
	cout << OK_BARS_COLOR;
	cout << "-------------------------------------------" << endl;
	cout << RESET_COLOR;
}

void Test::end(string argv) {
	cout << LOG_BARS_COLOR;
	cout << "===========================================" << endl;
	cout << LOG_COLOR;
	cout << "Fim do teste: " << argv << endl;
	cout << LOG_BARS_COLOR;
	cout << "===========================================" << endl;
	cout << RESET_COLOR;
}
