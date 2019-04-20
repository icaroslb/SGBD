#ifndef TABELA_H
#define TABELA_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Pagina.h"

class Tabela
{
protected:
	int numPag;
	std::vector<Pagina*> paginas;
	std::unordered_map<std::string, int> esquema;
	
public:
	Tabela (std::vector<std::string> novoEsquema);
	
	void inserirTupla (std::string *novaTupla);
	void inserirTupla (Registro *novaTupla);
	std::vector<Pagina*> obterPaginas ();
	
	int obterPosColuna(std::string nome);
	
	int numTuplasGeradas (); 	// GET soma dos registros das pags
	int numPagsGeradas ();	  	// GET numPag
	std::vector<std::string*> tuplasGeradas (); // GET resutaldo, transforma a table numa string unica
};

#endif //TABELA_H
