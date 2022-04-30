// Windows
#ifdef _WIN32
	#define RESET_COLOR 	""

	#define ERR_COLOR 	""
	#define ERR_BARS_COLOR	""

	#define OK_COLOR 	""
	#define OK_BARS_COLOR 	""

	#define LOG_COLOR  	""
	#define LOG_BARS_COLOR	""
	
	#define USR_COLOR  	""
	
// Linux
#elif __linux__
	#define RESET_COLOR 	"\x1B[0m"

	#define ERR_COLOR 	"\x1B[31m"
	#define ERR_BARS_COLOR	"\x1B[91m"

	#define OK_COLOR 	"\x1B[32m"
	#define OK_BARS_COLOR 	"\x1B[92m"

	#define LOG_COLOR  	"\x1B[33m"
	#define LOG_BARS_COLOR	"\x1B[93m"
	
	#define USR_COLOR  	"\x1B[96m"

// MacOS? e outros
#else 
	#define RESET_COLOR 	""

	#define ERR_COLOR 	""
	#define ERR_BARS_COLOR	""

	#define OK_COLOR 	""
	#define OK_BARS_COLOR 	""

	#define LOG_COLOR  	""
	#define LOG_BARS_COLOR	""
	
	#define USR_COLOR  	""
#endif
	
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Test {
	void begin(string agv);
	void fail(string agv);
	void sucess(string agv);
	void end(string agv);	
	
	ostream &out;
	
	public:
		Test(ostream &_out): out(_out) {} 
		void execute(string argv, bool (*func)(Test& test)) {		
			out << endl;
			begin(argv);
			if(func(*this))
				sucess(argv);
			else
				fail(argv);
			end(argv);
			out << RESET_COLOR << endl;	// Resetar cor
		}
		ostream& ok() {return out << OK_COLOR;} 
		ostream& log() {return out << LOG_COLOR;} 
		ostream& error() {return out << ERR_COLOR;} 
		ostream& comment() {return out << USR_COLOR;} 
		ostream& normal() {return out << RESET_COLOR;}
};

void Test::begin(string argv) {
	out << LOG_BARS_COLOR;
	out << "===========================================" << endl;
	out << LOG_COLOR;
	out << "Inicio do teste: " << argv << endl;
	out << LOG_BARS_COLOR;
	out << "===========================================" << endl;
	out << RESET_COLOR;
}

void Test::fail(string argv) {
	out << ERR_BARS_COLOR;
	out << "-------------------------------------------" << endl;
	out << ERR_COLOR;
	out << "Falha no teste: " << argv << endl;
	out << ERR_BARS_COLOR;
	out << "-------------------------------------------" << endl;
	out << RESET_COLOR;
}

void Test::sucess(string argv) {
	out << OK_BARS_COLOR;
	out << "-------------------------------------------" << endl;
	out << OK_COLOR;
	out << "Sucesso no teste: " << argv << endl;
	out << OK_BARS_COLOR;
	out << "-------------------------------------------" << endl;
	out << RESET_COLOR;
}

void Test::end(string argv) {
	out << LOG_BARS_COLOR;
	out << "===========================================" << endl;
	out << LOG_COLOR;
	out << "Fim do teste: " << argv << endl;
	out << LOG_BARS_COLOR;
	out << "===========================================" << endl;
	out << RESET_COLOR;
}
