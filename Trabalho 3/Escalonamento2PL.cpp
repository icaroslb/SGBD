
#include <vector>
#include <string>
#include "parser.h"

//#################################################################
// ESTRUTURAS DE DADOS
enum class Status {active, commited, aborted};


typedef struct Transaction{
	int id;
	Status status;
	int timestamp;
}TR;

enum class Mode {S, X};

typedef struct Lock_Request{
	int pageid;
	Mode mode;
	int tr;
	struct Lock_Request *prox;
}LOCK;

//#################################################################

//#####################################################################
// MAIN CLASSES
class Tr_Manager{
	std::vector<TR*> Tr_list;
public:

	int next_timestamp = 0;

	Tr_Manager(int N_transactions){
		Tr_list.resize(N_transactions);
	}
	~Tr_Manager();

	TR* get(int tr){
		return Tr_list[tr];
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

	int list_size(){
		return Tr_list.size();
	}
	void clear(int N_transactions){
		// Tr_list.clear();
	
		for (int i = 0; i < Tr_list.size(); ++i){
			free(Tr_list[i]);
			Tr_list[i] = NULL;
		}
		// Tr_list.resize(N_transactions);

	}

};


class Lock_Manager{

	std::vector<LOCK*> Lock_Table;// (N_transactions, NULL);
	std::vector<LOCK*> Wait_Q, Tail;  // Tail aponta para as caldas das listas encadeadas de Wait_Q
	Tr_Manager* tr_manager;
	int N_transactions, N_pages, serializable;
public:
	Lock_Manager();
	Lock_Manager(int ntransaction, int npages){						
		Lock_Table.resize(npages);
		Wait_Q.resize(npages);
		Tail.resize(npages);
		serializable = 1;

		tr_manager = new Tr_Manager(ntransaction);	
	}
	~Lock_Manager();


	// ########################################################################################################################################################################################
	// Tratamento de Deadlock
	void wound_wait(int Tr, int D, LOCK* newlock){
		printf("Wound-wait\n");
		TR thisTr = *tr_manager->get(Tr);
		TR QueuedTr = *tr_manager->get(Lock_Table[D]->tr);
		TR aux;
		for (LOCK* p = Lock_Table[D]; p != NULL; p = p->prox){
				aux = *tr_manager->get(p->tr);
				if(aux.timestamp > QueuedTr.timestamp) QueuedTr = aux;
		}
		// if(QueuedTr.id != thisTr.id){

			if(thisTr.timestamp > QueuedTr.timestamp){
				// Se a pagina que está requisitando o bloqueio for mais nova ela espera
				printf("wait\n");
				if(Tail[D] != NULL){
					Tail[D]->prox = newlock;
					Tail[D] = Tail[D]->prox;
				}
				else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
			}
			else{// Se for mais velha, faz o rollback da mais nova
				// ROLLBACK younger transaction
				TR younger;
				for(LOCK* p = Lock_Table[D]; p != NULL; p = p->prox)
					younger = *tr_manager->get(p->tr);
					if(thisTr.timestamp < younger.timestamp){
						printf("ROLLBACK! Transaction id: %d timestamp: %d || older id: %d timestamp: %d \n", younger.id, younger.timestamp, tr_manager->get(Tr)->id, tr_manager->get(Tr)->timestamp);
						tr_manager->update_status(younger.id, Status::aborted);
						U(younger.id);
					}
			}
		// }
		// else{
		// 	std::cout << "same tr"<< std::endl;
		// 	Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; 
		// }
	}



	void wait_die(int Tr, int D, LOCK* newlock){
		printf("	Wait-die:   ");

		TR thisTr = *tr_manager->get(Tr);
		TR QueuedTr = *tr_manager->get(Lock_Table[D]->tr);
		TR aux;
		for (LOCK* p = Lock_Table[D]; p != NULL; p = p->prox){
				aux = *tr_manager->get(p->tr);
				if(aux.timestamp < QueuedTr.timestamp) QueuedTr = aux;
		}

		if(thisTr.timestamp <= QueuedTr.timestamp){
			printf("Wait\n");
			if(Tail[D] != NULL){
				Tail[D]->prox = newlock;
				Tail[D] = Tail[D]->prox;
			}
			else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
		}else{
			printf("Die - ROLLBACK! Transaction id: %d timestamp: %d \n", thisTr.id, thisTr.timestamp);
			tr_manager->update_status(Tr, Status::aborted);
			U(Tr);
		}		 		

	}




	// ############################################################################################################################################################################################

								
	bool LS(int Tr, int D){		//insere um bloqueio no modo compartilhado na Lock_Table se puder,
								// senao insere um Lock_Request da transacao Tr na Wait_Q de D
		if(Lock_Table.size() < D){ 	
			printf("	ERROR, PAGE OUT OF RANGE, D=%d\n", D);
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
		else{		// senao, a pagina esta bloqueada
			
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
		else{
			// Tratamento de deadlock
			wait_die(Tr, D, newlock);
			return false;
		}

	}  


	void U(int Tr){  		
	//apaga os bloqueios da transacao Tr da Lock_Table
	// note que se existir mais de uma pagina bloqueando a pagina ela permanecera bloqueada
		LOCK* aux, *request, *ant, **p;
		// printf("SIZE %d\n", size);
		// Percorre a Lock_Table desbloqueando as paginas 
		// printf("entrou\n");
		



		for (int D = 0; D < Lock_Table.size(); ++D){
			
			aux = NULL; ant = NULL;
			p = &Lock_Table[D];
			while ((*p) != NULL && p != NULL){
				if((*p)->tr == Tr){
					printf("	Unlocking page number %d\n", (*p)->pageid);
					aux = (*p);
					// if(ant != NULL) ant->prox = (*p)->prox;
					
					(*p) = (*p)->prox;
					free(aux);
				}
				else {
					// ant = (*p);
					p = &((*p)->prox); 
				}
			}


		}




		// printf("FOR 1 OK\n");

		// Percorre a Wait_Q apagando pedidos de Tr
		aux = NULL; 
		for (int i = 0; i < Wait_Q.size(); ++i){
		// printf("NADA FOR 2\n");

			request = Wait_Q[i];
			ant = request;
			while( request != NULL ){
				// printf("NADA WHILE\n");
				if(request->tr == Tr){
					aux = request;
					if(request == Wait_Q[i]){
						Wait_Q[i] = Wait_Q[i]->prox;
						request = request->prox;	
					}
					else{
						serializable = 0;
						ant->prox = request->prox;
						request = request->prox;
					}
					if(aux == Tail[i]) Tail[i] = ant;
					free(aux);

				}
				else{
					ant = request;
					request = request->prox;
				}
			}
		}

		for (int D = 0; D < Lock_Table.size(); ++D)
			alock(D);

	}


	void U(int Tr, int D){
		LOCK* aux = NULL, *ant = NULL;
		LOCK **p = &Lock_Table[D];
		while (*p != NULL && p != NULL){
			if((*p)->tr == Tr){
				printf("Unlocking page number %d\n", (*p)->pageid);
				aux = (*p);
				(*p) = (*p)->prox;
				free(aux);
			}
			else {
				ant = (*p);
				p = &((*p)->prox); 
			}
		}

		alock(D);

	}

	void alock(int D){
		// Faz andar a fila Wait_Q 
		if(Lock_Table[D] == NULL) {
			if(Wait_Q[D]!= NULL){
				Lock_Table[D] = Wait_Q[D];
				Wait_Q[D] = Wait_Q[D]->prox;
				Lock_Table[D]->prox = NULL;

				if(Lock_Table[D]->mode == Mode::S){
					LOCK *last = Lock_Table[D], *slock;
					while(Wait_Q[D] != NULL && Wait_Q[D]->mode == Mode::S){
						slock = Wait_Q[D];
						Wait_Q[D] = Wait_Q[D]->prox;
						last->prox = slock;
						last = last->prox;
						last->prox = NULL;
					}

				} 			
			}
 		}
	}



	void start(OP next_operation){
		// inicia uma operacao, que pode ser de inicializacao de transacao, de leitura, de escrita ou um commit.
		// operacoes de transacoes terminadas e abortadas serao ignoradas 
		if (next_operation.ope == Type::BT || tr_manager->get(next_operation.id)->status != Status::commited && tr_manager->get(next_operation.id)->status != Status::aborted){
			
			// Caso iniciar nova transacao
			if (next_operation.ope == Type::BT){ 
				printf("Starting Transaction %d\n", next_operation.id);
			  	TR new_transaction;
			  	new_transaction.id = next_operation.id;
			  	new_transaction.status = Status::active;
			  	new_transaction.timestamp = tr_manager->next_timestamp;
			  	tr_manager->next_timestamp += 1;

			  	//adiciona uma nova transacao a lista Tr_list com status ativa 
			  	tr_manager->insert(new_transaction);
			  	std::cout << "timestamp: " << tr_manager->get(next_operation.id)->id << " | global timestamp: " << tr_manager->next_timestamp << std::endl;
			}

			// Caso operacao de leitura, chama a funcao LS
			else if(next_operation.ope == Type::R){
				printf("Transaction id: %d | Reading page %d \n", next_operation.id, next_operation.item);
				int error = LS(next_operation.id, next_operation.item);
				if(error) printf("Falha na transacao\n");
			}
			// Caso operacao de escrita, chama a operacao LX
			else if(next_operation.ope == Type::W){
				printf("Transaction id: %d | Writing page %d \n", next_operation.id, next_operation.item);
				int error = LX(next_operation.id, next_operation.item);
				if(error) printf("Falha na transacao\n");
			}
			// Caso fim de transacao, chama a funcao U, liberando 
			else if(next_operation.ope == Type::CM){
				printf("Transaction id: %d | Commiting...\n", next_operation.id);
				U(next_operation.id);
				tr_manager->update_status(next_operation.id, Status::commited);

			}
		}
		else {
			std::cout << "Operation " << next_operation.id  << " is already " << ((tr_manager->get(next_operation.id)->status == Status::commited) ?  "commited" : "aborted") << std::endl;
		}
	
	}


	void scheduler(std::vector<std::vector<OP>> &story){     //Le o arquivo e resolve deadlock atravez do Wait_Die.
		TR* p;
		LOCK *l, *qd;
		for (int i = 0; i < story.size(); ++i){
			std::cout << std::endl << "Historia " << i << std::endl; 
			for (int j = 0; j < story[i].size(); ++j){
				start(story[i][j]);
				// printf("i = %d j = %d\n", i, j);
				

			}

			if(!serializable) std::cout <<  "################################################"  <<  std::endl << "THIS STORY IS NOT SERIALIZABLE!" << std::endl <<  "###########################################" << std::endl;
			
			// int ntransaction = N_transactions;
			// int npages = N_pages;
			// Lock_Table.swap(std::vector<LOCK*>);
			// // Lock_Table.clear();
			// Lock_Table.resize(npages);
			



			// Wait_Q.swap(std::vector<LOCK*>);
			// // Wait_Q.clear();
			// Wait_Q.resize(npages);
			
			// Tail.swap(std::vector<LOCK*>);
			// // Tail.clear();
			// Tail.resize(npages);
			// printf("lt %d wq %d t %d\n", (int)Lock_Table.size(), (int)Wait_Q.size(), (int)Tail.size() );		
			// tr_manager->clear(ntransaction);		

		// PRINT #####################################################################################################################################################
		p = NULL;
		std::cout << "Transactions" << std::endl;
		for (int k = 0; k < 2; ++k){
			std::cout << "< ";
			p = tr_manager->get(k);
			if(p!=NULL) std::cout << "T" <<  p->id << ", status=" << ((p->status == Status::active) ? "active" : ((p->status == Status::commited) ? "commited" : "aborted" )) << ", timestamp=" << p->timestamp << " | " ;
			std::cout << " >" << std::endl;		
		}
		l = NULL;
		std::cout << "Lock Table" << std::endl;
		for (int k = 0; k < Lock_Table.size(); ++k){
			std::cout << "< ";
			l = Lock_Table[k];
			for (; l != NULL; l = l->prox){
				if(l != NULL) std::cout << "pageid=" << l->pageid << ", mode=" << ((l->mode == Mode::X) ? 'X' : 'S') << ", T" << l->tr << " | " ;
				
			}
			

			std::cout << " >" << std::endl;		
		}


		qd = NULL;
		std::cout << "Wait Queue" << std::endl;
		for (int k = 0; k < Wait_Q.size(); ++k){
			std::cout << "< ";
			qd = Wait_Q[k];
			for (; qd != NULL; qd = qd->prox){
				if(qd != NULL) std::cout << "pageid"<< qd->pageid << ", mode=" << ((qd->mode == Mode::X) ? 'X' : 'S') << ", T" << qd->tr << " | " ;
				
			}
			

			std::cout << " >" << std::endl;		
		}

		// printf("npages %d ntr %d\n", N_pages, N_transactions);

		// ########################################################################################################################################################
		// LIMPANDO VETORES 
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


			for (int D = 0; D < Lock_Table.size(); ++D){
				LOCK* p = Lock_Table[D], *aux;
				while(p != NULL){
					aux = p;
					p = p->prox;
					free(aux);
				}
				Lock_Table[D] = NULL;
			}
			for (int D = 0; D < Wait_Q.size(); ++D){
				LOCK* p = Wait_Q[D], *aux;
				while(p != NULL){
					aux = p;
					p = p->prox;
					free(aux);
				}
				Wait_Q[D] = NULL;
				Tail[D] = NULL;
			}
			// Lock_Table.resize(N_pages);
			// Wait_Q.resize(N_pages);
			// Tail.resize(N_pages);
			tr_manager->clear(N_transactions);
			serializable = 1;
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




		}



	}
};

//#########################################################################################################################################################

int main(int argc, char const *argv[]){
	std::vector<OP> v;
	
	// Estoria A
	OP op1  = {0, Type::BT, -1};
	OP op2  = {1, Type::BT, -1};
	OP op3  = {1, Type::R, 0};
	OP op4  = {0, Type::R, 1};
	OP op5  = {0, Type::W, 1};
	OP op6  = {1, Type::R, 1 };
	OP op7  = {0, Type::W, 2};
	OP op8  = {0, Type::CM, -1};
	OP op9  = {1, Type::W, 1};
	OP op10 = {1, Type::R, 2};
	OP op11 = {1, Type::W, 2};
	OP op12 = {1, Type::CM, -1};

	// Estoria B
	OP op23  = {0, Type::BT, -1};
	OP op24  = {1, Type::BT, -1};

	OP op13  = {1, Type::R, 0};
	OP op14  = {0, Type::R, 1};
	OP op15  = {0, Type::W, 1 };
	OP op16  = {1, Type::R, 1 };
	OP op17  = {0, Type::W, 2  };
	OP op18  = {0, Type::CM, -1};
	OP op19  = {1, Type::W, 1};
	OP op20  = {1, Type::R, 2};
	OP op21  = {1, Type::W, 2};
	OP op22  = {1, Type::CM, -1};



	OP op25  = {1, Type::BT, -1};
	OP op26  = {2, Type::BT, -1};

	OP op27  = {1, Type::R, 1};
	OP op28  = {2, Type::R, 0};
	OP op29  = {1, Type::R, 0};







	v.push_back(op1);
	v.push_back(op2);
	v.push_back(op3);
	v.push_back(op4);
	v.push_back(op5);
	v.push_back(op6);
	v.push_back(op7);
	v.push_back(op9);
	v.push_back(op10);
	v.push_back(op11);
	v.push_back(op12);
	// v.push_back(op8);

	std::vector<OP> w;
	w.push_back(op23);
	w.push_back(op24);
	w.push_back(op13);
	w.push_back(op14);
	w.push_back(op15);
	w.push_back(op16);
	w.push_back(op17);
	w.push_back(op18);
	w.push_back(op19);
	w.push_back(op20);
	w.push_back(op21);
	w.push_back(op22);



	Lock_Manager *lm = new  Lock_Manager(3, 4);
	// Lock_Manager lm2(v.size(),3);
	std::vector<std::vector<OP> > v1;
	v1.push_back(v);
	v1.push_back(w);
	lm->scheduler(v1);


	return 0;
}


// enum class Type {BT, R, W, CM};
// typedef struct Operation{
//     int  id;
//     Type ope;
//     char item;
// }OP;