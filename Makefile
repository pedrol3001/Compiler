# Variaveis como argumentos

# Diretorio do arquivo .l
LEXICAL_DIR = ./lexical/
# Nome do arquivo .l
LEXICAL_NAME = lexer.l

# Diretorio do arquivo .y
SINTATICAL_DIR = ./sintatical/
# Nome do arquivo .y
SINTATICAL_NAME = parser.y

TEST_DIR = ./tests/
TEST_NAME = test1.txt

# Regras a serem usadas

all: build clear

test: all
	./compiler $(TEST_DIR)$(TEST_NAME)

# Regras auxiliares
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



