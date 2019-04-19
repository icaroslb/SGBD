#include "../headers/Registro.h"

Registro::Registro (std::string *novaTupla) {
	colunas = novaTupla;
}

Registro::Registro (int tam) {
	colunas = (std::string*)malloc(sizeof(std::string)*tam);
}

void Registro::inserirRegistro (int col, std::string registro) {
	colunas[col] = registro;
}

std::string Registro::operator[] (int pos) {
	return colunas[pos];
}

std::string Registro::obterColuna (int pos) {
	return (*this)[pos];
}
