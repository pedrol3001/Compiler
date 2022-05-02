# Make file para executar o teste
E = e
CXX = g++ 
STD = -std=c++17

ifeq ($(debug),yes)
	DEBUG = -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector
else
	DEBUG = 
endif
FLAGS = -g $(STD) $(DEBUG)

TEST = test.cpp

SRC = Src

MAIN = compiler.cpp
CPP = $(wildcard $(SRC)/*/*.cpp)  $(wildcard $(SRC)/*/*/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(CPP))
H = $(patsubst %.cpp,%.h,$(CPP)) $(SRC)/Tipos.h

#$(info $(CPP))
#$(info $(H))
#$(info $(OBJ))

# Construcao de arquivos objeto
define make_obj =
$(1): 
	$(CXX) -c $(FLAGS) $(patsubst %.o, %.cpp, $(1)) -o $(1)
endef
$(foreach obj,$(OBJ),$(eval $(call make_obj,$(obj)))) 

# Geracao do .y do bison
Y = $(wildcard $(SRC)/*/*.y)
build_bison:
	cd "$(SRC)/Sintatico" && bison -d "../../$(Y)" --output="../bison.c"  --header="../bison.h" 

#  Geracao do .l do flex
L = $(wildcard $(SRC)/*/*.l)
build_flex: 
	cd "$(SRC)/Lexico" && flex "../../$(L)"  


# Processo de compilacao
build: build_bison build_flex $(OBJ)
	$(CXX) $(SRC)/$(MAIN) $(OBJ) $(FLAGS) -o compiler
	
# Limpeza de arquivos objeto
clear:
	rm -f $(OBJ)

# ===============================================

# Compilacao sem recompilar objetos
fast: build

# Compilacao 
compile: clear build	

# ===============================================
TESTS = $(patsubst %.cpp,%,$(wildcard Tests/*/*.cpp))
define execute_test =
test_$(1): 
	$(CXX) $(OBJ) $(FLAGS) $(1).cpp -o $(1)_teste_
	./$(1)_teste_ $(1)
endef
$(foreach test,$(TESTS),$(eval $(call execute_test,$(test)))) 

test: compile $(foreach test,$(TESTS),test_$(test))


	
	
