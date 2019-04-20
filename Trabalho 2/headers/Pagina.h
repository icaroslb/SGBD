#ifndef PAGINA_H
#define PAGINA_H

#include <string>
#include <vector>
#include "Registro.h"

class Pagina {
protected:
	int tam;
	std::vector<Registro*> registros;	

public:
	Pagina(int tamPagina);

	void inserirReg (std::string *reg);
	void inserirReg (Registro *novaTupla);
	bool cheia ();
	int quantRegistros ();
	std::vector<Registro*> obterRegistros ();
};

#endif //PAGINA_H
