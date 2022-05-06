#ifndef Assembly_h
#define Assembly_h

#include <string>
#include "../Codigo/Codigo.h"

struct Assembly: public Code::Codigo {
	Assembly(bool _ignore);
	virtual ~Assembly();
	virtual std::string str()=0;
};

#endif
