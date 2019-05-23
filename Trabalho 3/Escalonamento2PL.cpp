#include <vector>
#include <string>

#define N_PAGES 100

//#################################################################
// ESTRUTURAS DE DADOS
enum class Status {active, commited, aborted};

typedef struct Transacao{
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
	
	std::vector<TR> Tr;

public:
	Tr_Manager();
	~Tr_Manager();
			
};


class Lock_Manager{

	std::vector<LOCK*> Lock_Table;// (N_PAGES, NULL);
	// std::vector< std::vector<LOCK> > Wait_Q;
	std::vector<LOCK*> Wait_Q, Tail;

public:
	Lock_Manager(){
		// Lock_Table = new std::vector<LOCK*>(N_PAGES, NULL);
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
				if(Tail[D] != NULL){
					Tail[D]->prox = newlock;#include <vector>
#include <string>

#define N_PAGES 100

//#################################################################
// ESTRUTURAS DE DADOS
enum class Status {active, commited, aborted};

typedef struct Transacao{
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
	
	std::vector<TR> Tr;

public:
	Tr_Manager();
	~Tr_Manager();
			
};


class Lock_Manager{

	std::vector<LOCK*> Lock_Table;// (N_PAGES, NULL);
	// std::vector< std::vector<LOCK> > Wait_Q;
	std::vector<LOCK*> Wait_Q, Tail;

public:
	Lock_Manager(){
		// Lock_Table = new std::vector<LOCK*>(N_PAGES, NULL);
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
				if(Tail[D] != NULL){
					Tail[D]->prox = newlock;
					Tail[D] = Tail[D]->prox;
				}
				else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
			}
			return false;
		}
	}

	void LX(int Tr, int D);  			//insere um bloqueio no modo exclusivo na Lock_Table...

	void U(int Tr, int D);  //apaga o bloqueio da transacao Tr sobre o item D da Lock_Table

	void escalonar(std::string path);
};

//####################################################################

int main(int argc, char const *argv[]){
	

	return 0;
}
					Tail[D] = Tail[D]->prox;
				}
				else { Wait_Q[D] = newlock; Tail[D] = Wait_Q[D]; }
			}
			return false;
		}
	}

	void LX(int Tr, int D);  			//insere um bloqueio no modo exclusivo na Lock_Table...

	void U(int Tr, int D);  //apaga o bloqueio da transacao Tr sobre o item D da Lock_Table

	void escalonar(std::string path);
};

//####################################################################

int main(int argc, char const *argv[]){
	

	return 0;
}
