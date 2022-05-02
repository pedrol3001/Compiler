#ifndef tresEnderecos_h
#define tresEnderecos_h

#include "../Assembly/Assembly.h"

#include "../../Token/Token.h"

#include <string>
#include <list>
#include <vector>

namespace Addr3{
	struct Instrucao {
		static long long int count;
		static long long int linha();
		
		int size;	
		
		Instrucao(int _size);
		virtual ~Instrucao();
		virtual std::list<Assembly*> gera_codigo()=0;
	};

	// Declaracao ==================================

	struct Aloca: public Instrucao {
		int bytes;			
		std::list<Assembly*> gera_codigo();	
		Aloca(int _bytes); 	
	};
	
	struct Desaloca: public Instrucao {
		int bytes;			
		std::list<Assembly*> gera_codigo();	
		Desaloca(int _bytes); 	
	};

	// Operacao ====================================

	struct Operacao: public Instrucao {
		Token dst;
		std::vector<Token> op;	// No maximo 2, ex: dst = op1 . op2
		
		virtual ~Operacao();
		Operacao(Token _dst, Token _op); 
		Operacao(Token _dst, Token _op1, Token _op2); 
		Operacao(Token _dst, std::vector<Token>& _op); 	
		virtual std::list<Assembly*> gera_codigo()=0;	
	};
	
	struct Adicao: public Operacao {
		Adicao(Token _dst, Token _op1, Token _op2);
		std::list<Assembly*> gera_codigo();		
	};
	struct Multiplicacao: public Operacao {
		Multiplicacao(Token _dst, Token _op1, Token _op2);
		std::list<Assembly*> gera_codigo();	
	};
	struct Subtracao: public Operacao {
		Subtracao(Token _dst, Token _op1, Token _op2);
		std::list<Assembly*> gera_codigo();	
	};
	struct Divisao: public Operacao {
		Divisao(Token _dst, Token _op1, Token _op2);
		std::list<Assembly*> gera_codigo();	
	};
	struct Atribuicao: public Operacao {
		Atribuicao(Token _dst, Token _op);
		std::list<Assembly*> gera_codigo();	
	};

	// Saltos ======================================

	struct Salto: public Instrucao {
		long int instrucao;	// Linha de destiono
		Salto(long int instrucao);
		std::list<Assembly*> gera_codigo();	
	};
	
	// Saltos condicionais =========================
	
	// Chamada de funcao

	
	struct SaltoCondicional: public Instrucao {
		long int instrucao;	// Linha de destino=
		SaltoCondicional(long int _instrucao);
		
		virtual std::list<Assembly*> gera_codigo()=0;		
	};
	
	struct Blt: public SaltoCondicional {
		Token op1, op2; 
		Blt(Token _op1, Token _op2, long int _instrucao);
		std::list<Assembly*> gera_codigo();		
	};
	
	/*
	struct Beq: public SaltoCondicional {=
		std::vector<Assembly> gera_codigo();		
	};
	struct Ble: public SaltoCondicional {
		std::vector<Assembly> gera_codigo();		
	};
	*/
}

#endif
