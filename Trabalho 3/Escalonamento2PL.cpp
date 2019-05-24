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
	std::vector<LOCK*> Wait_Q, Tail;  // Tail aponta para as caldas das listas encadeadas de Wait_Q
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


	// ##########################################################################
	// Tratamento de Deadlock
	void wait_die(int Tr, int D, LOCK* newlock){

		if(tr_manager->get(Tr).timestamp < tr_manager->get(Lock_Table[D]->tr).timestamp){
			// Se a pagina que está requisitando o bloqueio for mais nova ela espera
			if(Tail[D] != NULL){
				Tail[D]->prox = newlock;
				Tail[D] = Tail[D]->prox;
			}
			else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
		}
		else{// Se for mais velha, faz o rollback da mais nova
			// ROLLBACK younger transaction
			TR younger = tr_manager->get(Lock_Table[D]->tr);
			tr_manager->update_status(younger.id, Status::aborted);

		}
	}

	// ##########################################################################

								
	bool LS(int Tr, int D){		//insere um bloqueio no modo compartilhado na Lock_Table se puder,
								// senao insere um Lock_Request da transacao Tr na Wait_Q de D
		if(Lock_Table.size() < D){ 	
			printf("ERROR, PAGE OUT OF RANGE\n");
			return true;
		}
		// cria um pedido de bloqueio
		LOCK* newlock = (LOCK*)malloc(sizeof(LOCK));				
		newlock->tr = Tr;
		newlock->mode = Mode::S;
		newlock->pageid = D;
		newlock->prox = NULL;
	
		if(Lock_Table[D] == NULL){		// Significa que a pagina esta livre

			Lock_Table[D] = newlock;
			return false;
		}
		if(Lock_Table[D] != NULL){		// senao, a pagina esta bloqueada
			
			if(Lock_Table[D]->mode == Mode::S){  // se o bloqueio for do tipo compartilhado adiciona
				LOCK* aux = Lock_Table[D];   	 // a transacao como uma das que bloqueia a pagina D
				while(aux->prox != NULL) aux = aux->prox;
				aux->prox = newlock;
			}
			else{ 	// Se a pagina estiver bloqueada exclusivamente faz o controle de deadlock

				// ##########################################################################
				//  Wait-Die

				wait_die(Tr, D, newlock);


				// ##########################################################################


			}
			return false;
		}
	}

	bool LX(int Tr, int D){			//insere um bloqueio no modo exclusivo na Lock_Table
		if(Lock_Table.size() < D){ 
			printf("ERROR, PAGE OUT OF RANGE\n");
			return true;
		}

		// cria um pedido de bloqueio
		LOCK* newlock = (LOCK*)malloc(sizeof(LOCK));				
		newlock->tr = Tr;
		newlock->mode = Mode::X;
		newlock->pageid = D;
		newlock->prox = NULL;
		
		// Significa que a pagina esta livre
		if(Lock_Table[D] == NULL){
			Lock_Table[D] = newlock;
			return false;
		}
		// senao, a pagina esta bloqueada
		if(Lock_Table[D] != NULL){
			// Tratamento de deadlock
			wait_die(Tr, D, newlock);
			return false;
		}

	}  


	void U(int Tr){  		
	//apaga os bloqueios da transacao Tr da Lock_Table
	// note que se existir mais de uma pagina bloqueando a pagina ela permanecera bloqueada
		if(Lock_Table[D] == NULL) return;
		LOCK* aux, *request, *ant;
		// Percorre a Lock_Table desbloqueando as paginas 
		for (int i = 0; i < Lock_Table.size(); ++i){
			if(Lock_Table[i]->tr == Tr){
				aux = Lock_Table[i];
				Lock_Table[i] = Lock_Table[i]->prox;
				free(aux);
			}
		}

		// Percorre a Wait_Q apagando pedidos de Tr
		aux = NULL; 
		for (int i = 0; i < Wait_Q.size(); ++i){
			request = Wait_Q[i];
			ant = request;
			while( request != NULL ){
				if(request->tr == Tr){
					aux = request;
					if(request == Wait_Q[i]){
						Wait_Q[i] = Wait_Q[i]->prox;
						request = request->prox;	
					}
					else{
						ant->prox = request->prox;
						request = request->prox;
					}
					if(aux == Tail[i]) Tail = ant;
					free(aux);

				}
				else{
					ant = request;
					request = request->prox;
				}
			}
		}
	}

	bool start(OP next_operation){
		// inicia uma operacao, que pode ser de inicializacao de transacao, de leitura, de escrita ou um commit.
		// operacoes de transacoes terminadas e abortadas serao ignoradas 
		if ( tr_manager->get(next_operation.id).status != Status::commited && tr_manager->get(next_operation.id).status != Status::aborted){
			
			// Caso iniciar nova transacao
			if (next_operation.ope == Type::BT){ // tr_manager->Tr_map.find(next_operation.id) == tr_manager->Tr_map.end()){  esse cidigo comentado e pra testar se a transacao nao existe na map
			  	TR new_transaction;
			  	new_transaction.id = next_operation.id;
			  	new_transaction.status = Status::active;
			  	new_transaction.timestamp = tr_manager->next_timestamp;
			  	tr_manager->next_timestamp += 1;

			  	//adiciona uma nova transacao a lista Tr_list com status ativa 
			  	tr_manager->insert(new_transaction);
			}

			// Caso operacao de leitura, chama a funcao LS
			else if(next_operation.ope == Type::R){
				printf("Reading page %d | Transaction id: %d\n", next_operation.id, next_operation.item);
				int error = LS(next_operation.id, next_operation.item);
				if(error) printf("Falha na transacao\n");
			}
			// Caso operacao de escrita, chama a operacao LX
			else if(next_operation.ope == Type::W){
				printf("Writing page %d | Transaction id: %d\n", next_operation.id, next_operation.item);
				int error = LX(next_operation.id, next_operation.item);
				if(error) printf("Falha na transacao\n");
			}
			// Caso fim de transacao, chama a funcao U, liberando 
			else if(next_operation.ope == Type::CM){
				U(next_operation.id);
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