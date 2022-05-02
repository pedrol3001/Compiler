#ifndef tresEnderecos_h
#define tresEnderecos_h

#include "Gerador.h"

#include "../Token/Token.h"

#include <string>
#include <vector>

namespace Addr3{
	struct Instrucao {
		int size;	
		
		Instrucao(int _size);
		virtual std::vector<Assembly*> gera_codigo()=0;
	};

	// Declaracao ==================================

	struct Declaracao: public Instrucao {
		Token token;	// unico token por declaracao
		
		std::vector<Assembly*> gera_codigo();	
		Declaracao(Token _token); 	
	};

	// Operacao ====================================

	struct Operacao: public Instrucao {
		Token dst;
		std::vector<Token> op;	// No maximo 2, ex: dst = op1 . op2
		
		Operacao(Token _dst, std::vector<Token>& op); 	
		virtual std::vector<Assembly*> gera_codigo()=0;	
	};
	
	struct Adicao: public Operacao {
		Adicao(Token _dst, std::vector<Token>& op);
		std::vector<Assembly*> gera_codigo();		
	};
	struct Multiplicacao: public Operacao {
		Multiplicacao(Token _dst, std::vector<Token>& op);
		std::vector<Assembly*> gera_codigo();	
	};
	struct Subtracao: public Operacao {
		Subtracao(Token _dst, std::vector<Token>& op);
		std::vector<Assembly*> gera_codigo();	
	};
	struct Divisao: public Operacao {
		Divisao(Token _dst, std::vector<Token>& op);
		std::vector<Assembly*> gera_codigo();	
	};

	// Saltos ======================================

	struct Salto: public Instrucao {
		long int linha;	// Linha de destiono
		Salto(long int _linha);
		std::vector<Assembly*> gera_codigo();	
	};
	
	// Saltos condicionais =========================
	
	// Chamada de funcao

	
	struct SaltoCondicional: public Instrucao {
		long int linha;	// Linha de destino=
		SaltoCondicional(long int _linha);
		
		virtual std::vector<Assembly*> gera_codigo()=0;		
	};
	
	struct Beq: public SaltoCondicional {
		std::vector<Assembly*> gera_codigo();		
	};
	/*
	struct Blt: public SaltoCondicional {=
		std::vector<Assembly> gera_codigo();		
	};
	struct Ble: public SaltoCondicional {
		std::vector<Assembly> gera_codigo();		
	};
	*/
}

#endif
