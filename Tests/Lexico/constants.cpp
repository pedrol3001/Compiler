#include "../../Src/Lexico/Lexico.h"

#include <iostream>
#include "../test_format.cpp"

using namespace std;

// Código feito por Vitor
bool test(Test &tester)
{
	TabSim &ts = TabSim::getInstance();
	Lexico lexico("Resources/Lexico/constantes.txt");
	tester.normal() << lexico.size() << endl;

	tester.comment() << "\nTestando valores de constantes na tabela de simbolos" << endl;
	Token token;
	while (lexico >> token)
	{
		tester.normal() << "Enum: " << token() << "\tn: " << token.id() << endl;
	
		if (ts[token].has("StrAtt"))
		{
			StrAtt *str = (StrAtt *)ts[token]["StrAtt"];
			tester.normal() << str->str << endl;
		}
	
		if (ts[token].has("IdVal"))
		{
			IdVal *id = (IdVal *)ts[token]["IdVal"];
			tester.normal() << "IdVal: " << id->val << endl;
		}

		if (ts[token].has("IntVal"))
		{
			IntVal *it = (IntVal *)ts[token]["IntVal"];
			tester.normal() << "IntVal: " << it->val << endl;
		}
		
		if (ts[token].has("DoubleVal"))
		{
			DoubleVal *dou = (DoubleVal *)ts[token]["DoubleVal"];
			tester.normal() << "Double val: " << dou->val << endl;
		}
		
		if (ts[token].has("StringVal"))
		{
			StringVal *str = (StringVal *)ts[token]["StringVal"];
			tester.normal() << "String val: " << str->val << endl;
		}


		tester.comment() << "===================================" << endl;
	}
	return true;
}

int main(int argn, char *argv[])
{
	Test tester(cout);
	tester.execute(argv[1], test);
	return 0;
}
