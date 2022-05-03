#ifndef Sintatico_h
#define Sintatico_h

#include "../Lexico/Lexico.h"
#include "../Semantico/Bloco/Bloco.h"

#include <memory>

struct Sintatico {	
	bool analisar(Lexico &lexico);
	std::vector<std::shared_ptr<Bloco> > blocos();
	bool good();
	private:
		std::vector<std::shared_ptr<Bloco> > blocosGerados;
		bool ok=false;
};


#endif
