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
};

#endif //PROJECAO_H
