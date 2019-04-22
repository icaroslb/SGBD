#include "../headers/Projecao.h"
#include <iostream>

Operador::Operador (Tabela tab, std::vector<std::string> novoEsquema) : Tabela (novoEsquema){
	esquemaProjecao = novoEsquema;
	paginasProjecao = tab.obterPaginas();
	
	for(std::string i : novoEsquema){
		posColunas.push_back(tab.obterPosColuna(i));
	}
}

void Operador::executar (){
	std::string chave, *regAux;
	std::vector<Registro*> listaRegistros;
	std::unordered_map<std::string, std::string*> tabHash;
	int indice;
	
	for(Pagina *i : paginasProjecao){                                             //Iteraremos sobre páginas da tabela original
		listaRegistros = i->obterRegistros();                                       //Primeiro obtemos os registros da página
		
		for(Registro *j : listaRegistros){   
			indice = 0;
			
			regAux = new std::string[esquemaProjecao.size()];                         //Agora iteraremos sobre os registros da página
			for(int k : posColunas){                                                  //Preencheremos o registro e atualizaremos a chave
				regAux[indice++] = (*j)[k];
				chave += (*j)[k];
			}
			
			tabHash[chave] = regAux;                                                  //Colocamos cada registro na tabela hash
			chave.clear();                                                            //Zeramos a chave para usá-a novamente
		}
	}
	
	for(int i = 0; i < tabHash.bucket_count(); i++){                              //Iteramos sobre os buckets
		for(auto j = tabHash.begin(i); j != tabHash.end(i); j++){                   //Iteramos sobre os elementos dos buckets
			inserirTupla(j->second);
		}
	}
	
	return;
}

int Operador::numCols(){
	return esquemaProjecao.size();
}