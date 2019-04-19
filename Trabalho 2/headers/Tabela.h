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
	std::vector<Pagina*> obterPaginas ();
	
	int obterPosColuna(std::string nome);
};

#endif //TABELA_H
