# Variaveis como argumentos

# Diretorio do arquivo .l
LEXICAL_DIR = ./lexical/
# Nome do arquivo .l
LEXICAL_NAME = lexer.l

# Diretorio do arquivo .y
SINTATICAL_DIR = ./sintatical/
# Nome do arquivo .y
SINTATICAL_NAME = parser.y

TEST_DIR = ./Tests/
TESTS = $(wildcard $(TEST_DIR)*.txt)

# Regras a serem usadas

all: build clear

test: all $(foreach TEST,$(TESTS),rule_$(TEST))

# Regras auxiliares
define execute_test
rule_$(1): $(1)
	./compiler $(1)
endef
$(foreach TEST,$(TESTS),$(eval $(call execute_test,$(TEST))))

LEX = lex.yy
PARSER = parser.tab

# Compilacao final
build: $(LEX).c $(PARSER).c $(PARSER).h
	gcc -o compiler $(LEX).c $(PARSER).c -lfl

# Remocao dos arquivos .c e .h gerados
clear:
	rm -f $(LEX).c $(PARSER).c $(PARSER).h

# Construcao do analisador lexico
LEXICAL = $(LEXICAL_DIR)$(LEXICAL_NAME)
$(info $(LEXICAL))
$(LEX).c: $(LEXICAL)
	flex $(LEXICAL)

# Construcao do analisador sintatico
SINTATICAL_FLAGS = -Wcounterexamples
SINTATICAL = $(SINTATICAL_DIR)$(SINTATICAL_NAME)
$(PARSER).c: $(SINTATICAL)
	bison -d $(SINTATICAL) $(SINTATICAL_FLAGS)



