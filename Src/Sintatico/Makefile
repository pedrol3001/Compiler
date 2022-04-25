# Make file para executar o teste
E = e

CXX = g++ 
STD = -std=c++17
DEBUG = -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector

FLAGS = -g $(STD) $(DEBUG)

TEST = test.cpp

CPP = Sintatico.cpp ../Token/Token.cpp ../Lexico/Lexico.cpp ../TabelaDeSimbolos/TabelaDeSimbolos.cpp
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

Y = $(wildcard *.y)
build_bison:
	bison -d $(Y)

build: build_bison $(OBJ)
	
clear:
	rm -f $(OBJ)

test: clear build $(H)  
	$(CXX) $(TEST) $(FLAGS) $(OBJ) -o $(E)
	./$(E)
	
	
	
