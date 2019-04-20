#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include <vector>

class Registro{
protected:
	std::string *colunas;
public:
	Registro (std::string *novaTupla);
	Registro (int tam);
	
	void inserirRegistro (int col, std::string registro);
	
	std::string obterColuna (int pos);
	std::string operator[] (int pos);
	std::string* obterRegistro ();
};

#endif //REGISTRO_H
