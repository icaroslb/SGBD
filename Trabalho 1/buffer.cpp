#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <fstream>
#include <limits>

#define BUFFER buffer*
#define LRU 1
#define FIFO 2
#define CLOCK 3
#define MRU 4

using namespace std;

typedef struct BUF{
	int num;
	string linha;
	struct BUF *prox, *ant;
}buffer;

typedef struct FILA{
	BUFFER cabeca;
	BUFFER cauda;
}fila;

void menu(int politica);
void buscarLinha(map<int, BUFFER> *buff, string *nomeArquivo, fila *fil, int politica);
void mostrarBuffer(map<int, BUFFER> *buff);

void inserirFila(fila *fil, BUFFER novo);
void remover(map<int, BUFFER> *buff, fila *fil, int politica);
void removerLRUeFIFO(map<int, BUFFER> *buff, fila *fil);
void removerMRU(map<int, BUFFER> *buff, fila *fil);

void mudarPolitica(int *politica);

int main(){
	string nomeArquivo;
	map<int, BUFFER> buff;
	int opcao, politica = LRU;
	fila fil;
	
	fil.cabeca = NULL;
	fil.cauda = NULL;
	
	
	cout << "Gerenciador de buffer" << endl << endl;
	cout << "Insira o arquivo: ";
	cin >> nomeArquivo;
	scanf("%*c");
	
	do{
		system("clear");
		mostrarBuffer(&buff);
		do{
			menu(politica);
			cin >> opcao;
			scanf("%*c");
			cout << endl << endl;
			
			if(opcao < 1 || opcao > 5){
				cout << "Opção inexistente!!!";
				scanf("%*c");
			}
		}while(opcao < 1 || opcao > 5);
		
		system("clear");
		
		switch(opcao){
			case 1:
				buscarLinha(&buff, &nomeArquivo, &fil, politica);
			break;
			case 2:
				remover(&buff, &fil, politica);
			break;
			case 3: break;
			case 4:
				mudarPolitica(&politica);
			break;
			case 5: break;
			default: break;
		}
	}while(opcao != 5);
	
	return 0;
}

void menu(int politica){
	cout << "Política de subistituição: ";
	
	switch(politica){
		case LRU:
			cout << "LRU";
		break;
		case FIFO:
			cout << "FIFO";
		break;
		case CLOCK:
			cout << "CLOCK";
		break;
		case MRU:
			cout << "MRU";
		break;
		default: break;
	}
	
	cout << endl << endl << "Menu" << endl;
	cout << "1 - Buscar linha" << endl;
	cout << "2 - Remover buffer" << endl;
	cout << "3 - Mostrar status" << endl;
	cout << "4 - Mudar política de substituição" << endl;
	cout << "5 - Sair" << endl << endl;
	cout << "Escolha uma opção: ";
}

void buscarLinha(map<int, BUFFER> *buff, string *nomeArquivo, fila *fil, int politica){
	string valor;
	int num;
	BUFFER aux;
	
	cout << "Insira a linha desejada: ";
	cin >> num;
	scanf("%*c");
	
	map<int, BUFFER>::iterator achou = buff->find(num);
	
	if(achou != buff->end()){
		aux = achou->second;
		cout << "Valor encontrado:" << endl << "Numero: " << aux->num << endl << "Linha: " << aux->linha << endl << endl;
		
		if(politica != FIFO ){
			inserirFila(fil, aux);
		}
	}else{
		fstream arquivo(nomeArquivo->data(), fstream::in);
		
		if(buff->size() > 4){
			remover(buff, fil, politica);
		}
		
		for(int i = 0; i < num - 1; i++){
			arquivo.ignore(numeric_limits<streamsize>::max(),'\n');
		}
		
		getline(arquivo, valor);
		
		cout << "Valor encontrado:" << endl << "Numero: " << num << endl << "Linha: " << valor << endl << endl;
		
		aux = (BUFFER)malloc(sizeof(buffer));
		aux->num = num;
		aux->linha = valor;
		aux->prox = NULL;
		aux->ant = NULL;
		
		(*buff)[num] = aux;
		inserirFila(fil, aux);
	}
	
	scanf("%*c");
}

void mostrarBuffer(map<int, BUFFER> *buff){
	map<int, BUFFER>::iterator inicio = buff->begin(), fim = buff->end();
	map<int, BUFFER>::key_compare compar = buff->key_comp();
	BUFFER aux;
	
	cout << "----- buffer -----" << endl << endl;
	if(buff->size()){
		do{
			aux = inicio->second;
			cout << "Numero: " << aux->num << endl << aux->linha << endl << endl;
			inicio++;
		}while(inicio != fim);
	}
	cout << "------------------" << endl << endl;
}

void inserirFila(fila *fil, BUFFER novo){
	if(novo->prox != NULL || novo->ant != NULL){
		if(novo->prox != NULL){
			(novo->prox)->ant = novo->ant;
		}else{
			fil->cabeca = novo->ant;
		}
		if(novo->ant != NULL){
			(novo->ant)->prox = novo->prox;
		}else{
			fil->cauda = novo->prox;
		}
	}
	
	if(fil->cabeca == NULL && fil->cauda == NULL){
		novo->prox = NULL;
		novo->ant = NULL;
		
		fil->cabeca = novo;
		fil->cauda = novo;
	}else{
		novo->prox = fil->cauda;
		novo->ant = NULL;
		
		fil->cauda->ant = novo;
		
		fil->cauda = novo;
	}
}

void remover(map<int, BUFFER> *buff, fila *fil, int politica){
	if(fil->cabeca == NULL && fil->cauda == NULL){
		cout << "O buffer está vazio!!!" << endl << endl;
	}else{
		switch (politica){
			case LRU:
				removerLRUeFIFO(buff, fil);
			break;
			case FIFO:
				removerLRUeFIFO(buff, fil);
			break;
			case CLOCK:
			break;
			case MRU:
				removerMRU(buff, fil);
			break;
			default: break;
		}
	}
}

void removerLRUeFIFO(map<int, BUFFER> *buff, fila *fil){
	BUFFER aux = fil->cabeca;
	
	fil->cabeca = aux->ant;
	
	if(aux->ant != NULL){
		(aux->ant)->prox = NULL;
	}else{
		fil->cauda = NULL;
	}
	
	cout << "Resgitro que será removido:" << endl << "Linha: " << aux->num << endl << "Valor: " << aux->linha << endl;
	
	scanf("%*c");
	
	buff->erase(aux->num);
	free(aux);
}

void removerMRU(map<int, BUFFER> *buff, fila *fil){
	BUFFER aux = fil->cauda;
	
	fil->cauda = aux->prox;
	
	if(aux->prox != NULL){
		(aux->prox)->ant = NULL;
	}else{
		fil->cauda = NULL;
	}
	
	cout << "Resgitro que será removido:" << endl << "Linha: " << aux->num << endl << "Valor: " << aux->linha << endl;
	
	scanf("%*c");
	
	buff->erase(aux->num);
	free(aux);
}

void mudarPolitica(int *politica){
	int opcao;
	
	do{
		cout << "1 - LRU" << endl;
		cout << "2 - FIFO" << endl;
		cout << "3 - CLOCK" << endl;
		cout << "4 - MRU" << endl << endl;
		cout << "Escolha uma opção: ";
		
		cin >> opcao;
		scanf("%*c");
	
		if(opcao < 1 || opcao > 4){
			cout << "Opção inexistente!!!";
			scanf("%*c");
			system("clear");
		}
	}while(opcao < 1 || opcao > 4);
	
	*politica = opcao;
}
