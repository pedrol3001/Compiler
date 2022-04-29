#include "../test_format.cpp"

bool test() {
	return true;
}

int main(int argn, char *argv[]) {
	Test::execute(argv[1],test);
	return 0;
}
