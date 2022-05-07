#ifndef tresEnderecos_h
#define tresEnderecos_h

#include "../Assembly/Assembly.h"
struct Corretor;
#include "../Codigo/Codigo.h"

#include "../../Token/Token.h"


#include <string>
#include <list>
#include <memory>
#include <vector>


namespace Addr3{
	struct Instrucao: public Code::Codigo {
		Instrucao(std::string _classe, bool _ignore=false);
		Instrucao(std::string _classe, Token _op, bool _ignore=false);
		Instrucao(std::string _classe, Token _op1, Token _op2, bool _ignore=false);
		Instrucao(std::string _classe, Token _op1, Token _op2, Token _op3, bool _ignore=false);
		Instrucao(std::string _classe, std::vector<Token>& _ops, bool _ignore=false);
		virtual ~Instrucao();
		virtual std::list<std::shared_ptr<Assembly> > gera_codigo()=0;
		virtual void acao(Corretor& corretor);
		//virtual void acao(Otimizador& otimizador);
		public:
			std::string classe;		
			std::vector<Token> ops;
	};
	
	// Label ========================================
	struct Label: public Code::Label, public Instrucao {	// Label:
		Label(Token _label);
		std::list<std::shared_ptr<Assembly> > gera_codigo();
	};	
	
	// Inicializacao ================================
	
	struct SetZero: public Instrucao {
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		SetZero(); 	
	};
	
	struct SetGlobal: public Instrucao {
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		SetGlobal(); 	
	};
	
	struct SetLocal: public Instrucao {	
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		SetLocal(); 	
	};
	
	// Input/output =================================
	
	struct Read: public Instrucao {		// Read x;
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Read(Token _op); 	
	};
	
	struct Print: public Instrucao {	// Print x;
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Print(Token _op); 	
	};

	// Declaracoes ==================================

	struct AlocaGlobal: public Instrucao {	
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		AlocaGlobal(Token _op); 
		void acao(Corretor& corretor);	
	};

	struct Aloca: public Instrucao {	// Aloca x;
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Aloca(Token _op); 	
		void acao(Corretor& corretor);
	};
	
	struct Desaloca: public Instrucao {	// Desaloca x;
		Token op;			
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
		Desaloca(Token _op); 	
		void acao(Corretor& corretor);
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
	struct Atribuicao: public Operacao {	// x = y ou temp = (x=y)
		Atribuicao(Token _dst, Token _op);
		Atribuicao(Token _ret, Token _dst, Token _op);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	// Ponteiros=====================================
	
	struct LoadFromRef: public Instrucao {	// x = *y
		Token dst, pointer;
		LoadFromRef(Token _dst, Token _pointer);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	struct StoreInRef: public Instrucao {	// *x = y
		Token src, pointer;
		StoreInRef(Token _src, Token _pointer);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	// Chamada de funcao============================
	
	struct BeginCall: public Instrucao {	// Begin_call;
		BeginCall();
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	
	struct Param: public Instrucao {	// Param param;
		Token parametro;		
		Param(Token _parametro);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};

	struct Call: public Instrucao {		// Call function;
		Token ret,funcao;		
		Call(Token _ret, Token _funcao);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	
	struct Return: public Instrucao {		// Return ;
		Token ret;	
		bool has_value=false;	
		Return(Token _ret);
		Return();
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	
	// Saltos ======================================
	
	struct Goto: public Instrucao, public Code::Goto {	// Goto label
		Goto(Token _label);
		std::list<std::shared_ptr<Assembly> > gera_codigo();	
	};
	

	struct SaltoCondicional: public Instrucao, public Code::Goto {	// 
		SaltoCondicional(std::string _classe, Token _label);
		virtual std::list<std::shared_ptr<Assembly> > gera_codigo()=0;		
	};
	
	struct Beq: public SaltoCondicional {	// if op1 == op2 goto label
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
	
	// Outros ===========================================
	
	struct Comentario: public Instrucao {
		std::string str;
		Comentario(std::string _str);
		std::list<std::shared_ptr<Assembly> > gera_codigo();
	};
}

#include "../Corretor/Corretor.h"

#endif
