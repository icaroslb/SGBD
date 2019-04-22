#include "../headers/Tabela.h"

Tabela::Tabela (std::vector<std::string> novoEsquema){
	int tam = novoEsquema.size(), cont = 0;
	
	for(std::string i : novoEsquema){
		esquema[i] = cont;
		cont++;
	}
}

void Tabela::inserirTupla (std::string *novaTupla){
	Pagina *paginaInsercao;
	
	if (paginas.size() == 0  || (paginas.size() && (*paginas.back()).cheia())) {
		paginas.push_back(new Pagina(14));
	}
	
	paginaInsercao = paginas.back();
	
	paginaInsercao->inserirReg(novaTupla);
	
	return;
}

void Tabela::inserirTupla (Registro *novaTupla){
	Pagina *paginaInsercao;
	
	if (paginas.size() == 0 || (paginas.size() && (*paginas.back()).cheia())) {
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

int Tabela::numTuplasGeradas (){
	int numPaginas = paginas.size (), quantRegistros = 0;
	
	for(Pagina *i : paginas){
		quantRegistros += i->quantRegistros();
	}
	
	return quantRegistros;
}

int Tabela::numPagsGeradas (){
	return paginas.size();
}

std::vector<std::string*> Tabela::tuplasGeradas (){
	std::vector<std::string*> tuplasGeradas;
	std::vector<Registro*> registrosAux;
	
	for(Pagina *i : paginas){
		registrosAux = i->obterRegistros();
		
		for(Registro *j : registrosAux){
			tuplasGeradas.push_back(j->obterRegistro());
		}
	}
	
	return tuplasGeradas;
}
