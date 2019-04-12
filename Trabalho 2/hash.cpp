#include "hash.h"

template <typename T>
hash<T>::hash(){
	tabela = (T**)malloc(sizeof(T*) * 4);
	
	for(T **i : tabela){
		*i = (T*)malloc(sizeof(T) * 10);
		for(T *j : i){
			j = NULL;
		}
	}
	
	comp = 2;
}

template <typename T>
void hash<T>::inserir(T objeto){
	
}

template <typename T>
hash<T>::~hash(){
    //dtor
}

template <typename T>
unsigned int hash<T>::funcaoHash(T valor){
	unsigned int comparar = (0b1 << comp) - 1, respota;
	
	if(typeid(valor) == typeid(string)){
		for(int i = valor.size() - (comp/8); i < valor.size(); i++){
			comparar = comparar;
		}
	}else{
		comparar = valor;
	}
	
	return comparar & comparar;
}
