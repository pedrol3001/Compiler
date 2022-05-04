#ifndef Instrucao_h
#define Instrucao_h

#include <vector>
#include <memory>
#include "../../Token/Token.h"
#include "../../TabelaDeSimbolos/TabelaDeSimbolos.h"

namespace Code {
	struct Codigo {	// Usado para atualizar linhas polimorficamente
		long long int linha=-1;
		
		Codigo(bool _is_label);
		
		bool is_label();
		
		virtual void update(long long int _linha);	// Atualiza linha
		long long int getLinha();	// Atualiza linha
		virtual ~Codigo();	
		private:
			bool flag_is_label;	
	};

	struct Goto {	// Classes que precisam se referir a uma label
		Token label;
		Goto(Token _label);
		long long int readRef();
	};

	struct Label {
		Token label;
		Label(Token _label);
		void setRef(long long int linha);
	};
}

#endif
