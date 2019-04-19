#include "../headers/Pagina.h"
#include <iostream>

Pagina::Pagina(int tamPagina){
	tam = tamPagina;
}

void Pagina::inserirReg (std::string *reg){
	registros.push_back(new Registro(reg));
	return;
}

bool Pagina::cheia (){
	return registros.size() >= tam;
}

int Pagina::quantRegistros (){
	return registros.size();
}

std::vector<Registro*> Pagina::obterRegistros (){
	return registros;
}
