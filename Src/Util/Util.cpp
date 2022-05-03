#include "Util.h"


template <class T_Ptr>
void freeContainer(std::vector<T_Ptr>& ponteiros) {
	for(T_Ptr ponteiro: ponteiros)
		delete ponteiro;
}
