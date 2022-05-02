#ifndef Assembly_h
#define Assembly_h

#include <string>

struct Assembly {
	std::string nome;
	Assembly(std::string _nome);
	virtual std::string str()=0;
};

#endif
