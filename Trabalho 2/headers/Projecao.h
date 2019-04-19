#ifndef PROJECAO_H
#define PROJECAO_H

#include "Tabela.h"

class Operador : public Tabela{
protected:
	std::vector<Pagina*> paginasProjecao;
	std::vector<std::string> esquemaProjecao;
	std::vector<int> posColunas;
	
public:
	Operador (Tabela tabProjetada, std::vector<std::string> novoEsquema);
	
	void executar ();
	int numTuplasGeradas (); 	// GET soma dos registros das pags
	int numPagsGeradas ();	  	// GET numPag
	std::string* tuplasGeradas (); // GET resutaldo, transforma a table numa string unica
};

#endif //PROJECAO_H
