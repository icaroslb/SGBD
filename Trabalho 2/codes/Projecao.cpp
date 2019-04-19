#include "../headers/Projecao.h"

Operador::Operador (Tabela tab, std::vector<std::string> novoEsquema) : Tabela (novoEsquema){
	esquemaProjecao = novoEsquema;
	paginasProjecao = tab.obterPaginas();
	
	for(std::string i : novoEsquema){
		posColunas.push_back(tab.obterPosColuna(i));
	}
}

void Operador::executar (){
	std::string chave;
	std::vector<Registro*> listaRegistros;
	Registro regAux(esquemaProjecao.size());
	std::unordered_map<std::string, Registro> tabHash;
	
	for(Pagina *i : esquemaProjecao){                                             //Iteraremos sobre páginas da tabela original
		listaRegistros = i->obterRegistros();                                       //Primeiro obtemos os registros da página
		
		for(Registro *j : listaRegistros){                                          //Agora iteraremos sobre os registros da página
			for(int k : posColunas){                                                  //Preencheremos o registro e atualizaremos a chave
				regAux.inserirRegistro(k, (*j)[k]);
				chave += (*j)[k];
			}
			
			tabHash = {{chave, regAux}};                                              //Colocamos cada registro na tabela hash
			
			chave.clear();
		}
	}
	
	//A fazer (retirar da tabela hash e colocar nas páginas)
	
	return;
}

int Operador::numTuplasGeradas (){
	int numPaginas = paginas.size (), quantRegistros = 0;
	
	for(Pagina *i : paginas){
		quantRegistros += i->quantRegistros();
	}
	
	return quantRegistros;
}

int Operador::numPagsGeradas (){
	return paginas.size();
}

std::string* Operador::tuplasGeradas (){
	return NULL;
}
