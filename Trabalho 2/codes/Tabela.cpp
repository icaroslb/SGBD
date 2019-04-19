#include "../headers/Tabela.h"

Tabela::Tabela (std::vector<std::string> novoEsquema){
	int tam = novoEsquema.size(), cont = 0;
	
	for(std::string i : novoEsquema){
		esquema = {{i, cont}};
		cont++;
	}
}

void Tabela::inserirTupla (std::string *novaTupla){
	//Registro *novoRegistro = new Registro(novaTupla);
	Pagina *paginaInsercao;
	
	if (paginas.size() == 0 ){//|| (paginas.size() && (*paginas.end())->cheia())) {
		paginas.push_back(new Pagina(14));
	}
	
	paginaInsercao = paginas.back();
	
	paginaInsercao->inserirReg(novaTupla);
	
	return;
}

std::vector<Pagina*> Tabela::obterPaginas (){
	return paginas;
}

int Tabela::obterPosColuna(std::string nome){
	return esquema[nome];
}
