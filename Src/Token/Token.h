#ifndef Token_h
#define Token_h

struct Token {	
	Token();
	Token(int _tipo, long long int _index);
	
	long long id() const;	// Obter índice, mas pode ser desnecessário de qualquer forma.
	
	int operator()() const;	// Facilitar comparações

	int tipo;		
	long long int index;
};	

#endif
