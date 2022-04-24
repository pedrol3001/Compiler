# Make file para executar o teste
E = e

CXX = g++ 
STD = -std=c++17
FLAGS = -g $(STD)

TEST = test.cpp

CPP = TabelaDeSimbolos.cpp ../Token/Token.cpp
H = $(patsubst %.cpp,%.h,$(CPP)) ../Tipos.h
OBJ = $(patsubst %.cpp,%.o,$(CPP))

#$(info $(CPP))
#$(info $(H))
#$(info $(OBJ))

define make_obj =
$(1): 
	$(CXX) -c $(FLAGS) $(patsubst %.o, %.cpp, $(1)) -o $(1)
endef
	
$(foreach obj,$(OBJ),$(eval $(call make_obj,$(obj)))) 


build: $(OBJ)
	
clear:
	rm -f $(OBJ)

test: clear build $(H)  
	$(CXX) $(TEST) $(FLAGS) $(OBJ) -o $(E)
	./$(E)
	
	
	
