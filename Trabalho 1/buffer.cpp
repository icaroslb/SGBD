#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <limits>

#define BUFFER buffer*
#define LRU 1
#define FIFO 2
#define CLOCK 3
#define MRU 4

using namespace std;

typedef struct BUF{
	int num, posMemo;
	bool referenciado;
	string linha;
	struct BUF *prox, *ant;
}buffer;

typedef struct FILA{
	BUFFER cabeca;
	BUFFER cauda;
}fila;

void menu(int politica);
void buscarLinha(unordered_map<int, BUFFER> *buff, string *nomeArquivo, fila *fil, BUFFER **memo, int *corrente, int politica, int *hit, int *miss);
void mostrarBuffer(unordered_map<int, BUFFER> *buff, int hit, int miss);

void inserirFila(fila *fil, BUFFER novo);
void inserirMemo(BUFFER **memo, BUFFER novo);
void remover(unordered_map<int, BUFFER> *buff, fila *fil, BUFFER **memo, int *corrente, int politica);
void removerLRUeFIFO(unordered_map<int, BUFFER> *buff, fila *fil);
void removerMRU(unordered_map<int, BUFFER> *buff, fila *fil);
void removerClock(unordered_map<int, BUFFER> *buff, BUFFER **memo, int *corrente);

void mudarPolitica(int *politica, unordered_map<int, BUFFER> *buff, fila *fil, BUFFER **memo, int *corrente, int *hit, int *miss);

int main(){
	string nomeArquivo;
	unordered_map<int, BUFFER> buff;
	int opcao, politica = LRU, corrente = 0, hit = 0, miss = 0;
	fila fil;
	BUFFER *memo = (BUFFER*)malloc(sizeof(BUFFER) * 5);
	
	for(int i = 0; i < 5; i++){
		memo[i] = NULL;
	}
	
	fil.cabeca = NULL;
	fil.cauda = NULL;
	
	
	cout << "Gerenciador de buffer" << endl << endl;
	cout << "Insira o arquivo: ";
	cin >> nomeArquivo;
	scanf("%*c");
	
	do{
		system("clear");
		mostrarBuffer(&buff, hit, miss);
		do{
			menu(politica);
			scanf("%d%*c", &opcao);
			cout << endl << endl;
			
			if(opcao < 1 || opcao > 4){
				cout << "Opção inexistente!!!";
				scanf("%*c");
			}
		}while(opcao < 1 || opcao > 4);
		
		system("clear");
		
		switch(opcao){
			case 1:
				buscarLinha(&buff, &nomeArquivo, &fil, &memo, &corrente, politica, &hit, &miss);
			break;
			case 2:
				remover(&buff, &fil, &memo, &corrente, politica);
			break;
			case 3:
				mudarPolitica(&politica, &buff, &fil, &memo, &corrente, &hit, &miss);
			break;
			case 4: break;
			default: break;
		}
	}while(opcao != 4);
	
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
	cout << "3 - Mudar política de substituição" << endl;
	cout << "4 - Sair" << endl << endl;
	cout << "Escolha uma opção: ";
}

void buscarLinha(unordered_map<int, BUFFER> *buff, string *nomeArquivo, fila *fil, BUFFER **memo, int *corrente, int politica, int *hit, int *miss){
	string valor;
	int num, fim = 0;
	BUFFER aux;
	
	cout << "Insira a linha desejada: ";
	scanf("%d%*c", &num);
	
	unordered_map<int, BUFFER>::iterator achou = buff->find(num);
	
	if(achou != buff->end()){
		aux = achou->second;
		cout << "Valor encontrado no buffer:" << endl << "Numero: " << aux->num << endl << "Linha: " << aux->linha << endl << endl;
		
		(*hit)++;
		
		if(politica == CLOCK){
			aux->referenciado = true;
		}else if(politica != FIFO){
			inserirFila(fil, aux);
		}
	}else{
		fstream arquivo(nomeArquivo->data(), fstream::in);
		
		if(buff->size() > 4){
			remover(buff, fil, memo, corrente, politica);
		}
		
		for(int i = 0; i < num - 1 && fim != EOF; i++){
			arquivo.ignore(numeric_limits<streamsize>::max(),'\n');
		}
		
		if(getline(arquivo, valor)){
			cout << "Valor encontrado:" << endl << "Numero: " << num << endl << "Linha: " << valor << endl << endl;
			
			(*miss)++;
			
			aux = (BUFFER)malloc(sizeof(buffer));
			aux->num = num;
			aux->linha = valor;
			aux->referenciado = true;
			aux->prox = NULL;
			aux->ant = NULL;
			
			(*buff)[num] = aux;
			
			arquivo.close();
			
			if(politica == CLOCK){
				inserirMemo(memo, aux);
			}else{
				inserirFila(fil, aux);
			}
		}else{
			cout << "Valor não encontrado!" << endl << endl;
		}
	}
	
	scanf("%*c");
}

void mostrarBuffer(unordered_map<int, BUFFER> *buff, int hit, int miss){
	unordered_map<int, BUFFER>::iterator inicio = buff->begin(), fim = buff->end();
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
	cout << "buffer hit: " << hit << "\tbuffer miss: " << miss << endl << endl;
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

void inserirMemo(BUFFER **memo, BUFFER novo){
	for(int i = 0; i < 5; i++){
		if((*memo)[i] == NULL){
			novo->posMemo = i;
			(*memo)[i] = novo;
			return;
		}
	}
}

void remover(unordered_map<int, BUFFER> *buff, fila *fil, BUFFER **memo, int *corrente, int politica){
	if(!buff->size()){
		cout << "O buffer está vazio!!!" << endl << endl;
		scanf("%*c");
	}else{
		switch (politica){
			case LRU:
				removerLRUeFIFO(buff, fil);
			break;
			case FIFO:
				removerLRUeFIFO(buff, fil);
			break;
			case CLOCK:
				removerClock(buff, memo, corrente);
			break;
			case MRU:
				removerMRU(buff, fil);
			break;
			default: break;
		}
	}
}

void removerLRUeFIFO(unordered_map<int, BUFFER> *buff, fila *fil){
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

void removerMRU(unordered_map<int, BUFFER> *buff, fila *fil){
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

void removerClock(unordered_map<int, BUFFER> *buff, BUFFER **memo, int *corrente){
	BUFFER aux = NULL;
	bool achado = false;
	
	do{
		if((*memo)[*corrente] != NULL){
			
			if(((*memo)[*corrente])->referenciado){
				
				((*memo)[*corrente])->referenciado = false;
				(*corrente)++;
				
			}else{
				achado = true;
			}
			
		}else{
			(*corrente)++;
		}
		
		if(*corrente >= 5){
			*corrente = 0;
		}
	}while(!achado);
	
	aux = (*memo)[*corrente];
	
	cout << "Resgitro que será removido:" << endl << "Linha: " << aux->num << endl << "Valor: " << aux->linha << endl;
	
	scanf("%*c");
	
	buff->erase(aux->num);
	(*memo)[aux->posMemo] = NULL;
	free(aux);
}

void mudarPolitica(int *politica, unordered_map<int, BUFFER> *buff, fila *fil, BUFFER **memo, int *corrente, int *hit, int *miss){
	int opcao;
	BUFFER ant;
	BUFFER prox;
	
	do{
		cout << "1 - LRU" << endl;
		cout << "2 - FIFO" << endl;
		cout << "3 - CLOCK" << endl;
		cout << "4 - MRU" << endl << endl;
		cout << "Escolha uma opção: ";
		
		scanf("%d%*c", &opcao);
	
		if(opcao < 1 || opcao > 4){
			cout << "Opção inexistente!!!";
			scanf("%*c");
			system("clear");
		}
	}while(opcao < 1 || opcao > 4);
	
	*politica = opcao;
	
	buff->clear();
	
	if(fil->cabeca != NULL || fil->cauda != NULL){
		prox = fil->cabeca;
		while(prox != NULL){
			ant = prox;
			prox = prox->prox;
			free(ant);
		}
	}
	fil->cabeca = NULL;
	fil->cauda = NULL;
	
	for(int i = 0; i < 5; i++){
		if((*memo)[i] != NULL){
			free((*memo)[i]);
			(*memo)[i] = NULL;
		}
	}
	
	*hit = 0;
	*miss = 0;
	
}
