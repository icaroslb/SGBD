#include <vector>
#include <string>
#include <unordered_map>
#include "parser.h"

//#################################################################
// ESTRUTURAS DE DADOS
enum class Status {active, commited, aborted};


typedef struct Transaction{
	int id;
	Status status;
	double timestamp;
}TR;

enum class Mode {S, X};

typedef struct Lock_Request{
	int pageid;
	Mode mode;
	int tr;
	struct Lock_Request *prox;
}LOCK;

//#################################################################
// FIFO
// template <class T>
// class List{
// 	T chave;
// 	T *prox;
// 	T *ant;
// };


// template <class T>
// class FIFO{
// 	List <T> head, tail;

// public:
// 	FIFO();
// 	~FIFO();

// 	void insert();
// 	T remove();
// };
//#####################################################################
// MAIN CLASSES
class Tr_Manager{
	std::vector<TR*> Tr_list;
public:
	std::unordered_map<int, TR> Tr_map;
	int next_timestamp = 0;

	Tr_Manager(int N_transactions){
		Tr_list.resize(N_transactions);
	}
	~Tr_Manager();

	TR get(int tr){
		return *Tr_list[tr];
	}


	void insert(TR newtr){

		Tr_list[newtr.id] = (TR*)malloc(sizeof(TR));
		*Tr_list[newtr.id] = newtr;

	}	

	void remove(TR tr){
		Tr_list[tr.id] = NULL;
	}

	void update_status(int tr_id, Status new_status){
		Tr_list[tr_id]->status = new_status; 
	}
};


class Lock_Manager{

	std::vector<LOCK*> Lock_Table;// (N_transactions, NULL);
	std::vector<LOCK*> Wait_Q, Tail;
	Tr_Manager* tr_manager;
	int N_transactions, N_pages;
public:
	Lock_Manager(int ntransaction, int npages){						
		Lock_Table.resize(ntransaction);
		Wait_Q.resize(npages);
		Tail.resize(npages);

		tr_manager = new Tr_Manager(ntransaction);	
	}
	~Lock_Manager();

								
	bool LS(int Tr, int D){		//insere um bloqueio no modo compartilhado na Lock_Table se puder,
								// senao insere um Lock_Request da transacao Tr na Wait_Q de D
		if(Lock_Table.size() < D){ 
			printf("ERROR, PAGE OUT OF RANGE\n");
			return true;
		}
	
		LOCK* newlock = (LOCK*)malloc(sizeof(LOCK));				
		newlock->tr = Tr;
		newlock->mode = Mode::S;
		newlock->pageid = D;
		newlock->prox = NULL;
	
		if(Lock_Table[D] == NULL){

			Lock_Table[D] = newlock;
			return false;
		}
		if(Lock_Table[D] != NULL){
			
			if(Lock_Table[D]->mode == Mode::S){
				LOCK* aux = Lock_Table[D];
				while(aux->prox != NULL) aux = aux->prox;
				aux->prox = newlock;
			}
			else{

				// ##########################################################################
				//  Wait-Die


				if(tr_manager->get(Tr).timestamp < tr_manager->get(Lock_Table[D]->tr).timestamp){

					if(Tail[D] != NULL){
						Tail[D]->prox = newlock;
						Tail[D] = Tail[D]->prox;
					}
					else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
				}
				else{
					// ROLLBACK younger transaction
					TR younger = tr_manager->get(Lock_Table[D]->tr);
					tr_manager->update_status(younger.id, Status::aborted);

				}


				// ##########################################################################


			}
			return false;
		}
	}

	bool LX(int Tr, int D){			//insere um bloqueio no modo exclusivo na Lock_Table...
		if(Lock_Table.size() < D){ 
			printf("ERROR, PAGE OUT OF RANGE\n");
			return true;
		}

		LOCK* newlock = (LOCK*)malloc(sizeof(LOCK));				
		newlock->tr = Tr;
		newlock->mode = Mode::X;
		newlock->pageid = D;
		newlock->prox = NULL;
		
		if(Lock_Table[D] == NULL){
			Lock_Table[D] = newlock;
			return false;
		}
		if(Lock_Table[D] != NULL){
			if(Tail[D] != NULL){
				Tail[D]->prox = newlock;
				Tail[D] = Tail[D]->prox;
			}
			else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
			return false;
		}

	}  

	void U(int Tr, int D){  		//apaga o bloqueio da transacao Tr sobre o item D da Lock_Table
		if(Lock_Table[D] == NULL) return;

		LOCK* aux = Lock_Table[D];
		Lock_Table[D] = Lock_Table[D]->prox;
		free(aux);
	}

	bool start(OP next_operation){
		if ( tr_manager->get(next_operation.id).status != Status::commited && tr_manager->get(next_operation.id).status != Status::aborted){
			
			if (next_operation.ope == Type::BT){ // tr_manager->Tr_map.find(next_operation.id) == tr_manager->Tr_map.end()){   // se a transacao nao existir na map
			  	TR new_transaction;
			  	new_transaction.id = next_operation.id;
			  	new_transaction.status = Status::active;
			  	new_transaction.timestamp = tr_manager->next_timestamp;
			  	tr_manager->next_timestamp += 1;

			  	tr_manager->insert(new_transaction);
			}

			else if(next_operation.ope == Type::R){
				printf("Reading page %d | Transaction id: %d\n", next_operation.id, next_operation.item);
				int error = LS(next_operation.id, next_operation.item);
				if(error) printf("Falha na transacao\n");
			}
			else if(next_operation.ope == Type::W){
				printf("Writing page %d | Transaction id: %d\n", next_operation.id, next_operation.item);
				int error = LX(next_operation.id, next_operation.item);
				if(error) printf("Falha na transacao\n");
			}

			else if(next_operation.ope == Type::CM){
				U(next_operation.id, next_operation.item);
				tr_manager->update_status(next_operation.id, Status::commited);

			}
		}
	}


	void scheduler(std::vector<std::vector<OP>> story){     //Le o arquivo e resolve deadlock atravez do Wait_Die.
		for (int i = 0; i < story.size(); ++i){
			for (int j = 0; j < story[i].size(); ++j){
				start(story[i][j]);

				

			}
		}
	}
};

//####################################################################

int main(int argc, char const *argv[]){
	

	return 0;
}