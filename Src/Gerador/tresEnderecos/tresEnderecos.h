#ifndef tresEnderecos_h
#define tresEnderecos_h

#include "../Assembly/Assembly.h"
#include "../Codigo/Codigo.h"

#include "../../Token/Token.h"

#include <string>
#include <list>
#include <memory>
#include <vector>

namespace Addr3{
	struct Instrucao: public Code::Codigo  {
		std::string classe;		
		Instrucao(std::string _classe);
		virtual ~Instrucao();
		virtual std::list<std::shared_ptr<Assembly> > gera_codigo()=0;
	};
	
	// Label ========================================
	struct Label: public Code::Label, public Instrucao {
		Label(Token _label);
		std::list<std::shared_ptr<Assembly> > gera_codigo();
	};	

	// Input/output =================================
	
	struct Read: public Instrucao {
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Read(Token _op); 	
	};
	
	struct Print: public Instrucao {
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Print(Token _op); 	
	};

	// Declaracoes ==================================

	struct Global: public Instrucao {
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Global(Token _op); 	
	};

	struct Aloca: public Instrucao {
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Aloca(Token _op); 	
	};
	
	struct Desaloca: public Instrucao {
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Desaloca(Token _op); 	
	};

	// Operacao ====================================

	struct Operacao: public Instrucao {
		Token dst;
		std::vector<Token> op;	// No maximo 2, ex: dst = op1 . op2
		
		virtual ~Operacao();
		Operacao(std::string _classe, Token _dst, Token _op); 
		Operacao(std::string _classe, Token _dst, Token _op1, Token _op2); 
		Operacao(std::string _classe, Token _dst, std::vector<Token>& _op); 	
		virtual std::list<std::shared_ptr<Assembly> > gera_codigo()=0;	
	};
	
	struct Adicao: public Operacao {
		Adicao(Token _dst, Token _op1, Token _op2);
		std::list<std::shared_ptr<Assembly> > gera_codigo();		
	};
	struct Multiplicacao: public Operacao {
		Multiplicacao(Token _dst, Token _op1, Token _op2);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	struct Subtracao: public Operacao {
		Subtracao(Token _dst, Token _op1, Token _op2);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	struct Divisao: public Operacao {
		Divisao(Token _dst, Token _op1, Token _op2);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	struct Atribuicao: public Operacao {
		Atribuicao(Token _dst, Token _op);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	// Chamada de funcao============================
	
	struct BeginCall: public Instrucao {
		BeginCall();
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	
	struct Param: public Instrucao {
		Token parametro;		
		Param(Token _parametro);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};

	struct Call: public Instrucao {
		Token funcao;		
		Call(Token _funcao);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};

	// Saltos ======================================
	
	struct Goto: public Instrucao, public Code::Goto {
		Goto(Token _label);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	

	struct SaltoCondicional: public Instrucao, public Code::Goto {
		SaltoCondicional(std::string _classe, Token _label);
		virtual std::list<std::shared_ptr<Assembly> > gera_codigo()=0;		
	};
	
	struct Beq: public SaltoCondicional {
		Token op1, op2; 
		
		Beq(Token _op1, Token _op2, Token _label);
		std::list<std::shared_ptr<Assembly> > gera_codigo();		
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
