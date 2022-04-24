#ifndef Tipos_h
#define Tipos_h

enum Tipo{
	// Usado para debug
	NOT_INITIALIZED,
	
	// Comentários
	COMMENT,

	// Constantes
	C_STRING,C_CHAR,C_INT,C_FLOAT,
	
	// Palavras reservadas
		// Tipos
			VOID,
			// Letras
				STRING,CHAR,
			// Números
				INT,FLOAT,DOUBLE,
		// Comandos
		RETURN,OUTPUT,INPUT,
		// Construções
		IF,ELSE,WHILE,
		
	// Identificador
	ID,
	
	// Estruturas
		// Separadores
		SEMICOLON,COMMA,
		// Invólucros
		LPAREN,RPAREN,LBRACKET,RBRACKET,LBRACE,RBRACE,
	
	// Operações
		// Atribuição
		ASSIGN,
		// Operações aritméticas
		SUM,SUB,DIV,MUL,
		// Comparações
	 	LESS,LESSEQUAL,GREATER,GREATEREQUAL,EQUAL,NOTEQUAL,
};

using Tkn = Tipo;

#endif
