#include "../test_format.cpp"

bool test(Test& tester) {
	tester.comment() << "Nenhum teste foi executado!\n";
	return true;
}

int main(int argn, char *argv[]) {
	Test tester(cout);
	tester.execute(argv[1],test);
	return 0;
}
