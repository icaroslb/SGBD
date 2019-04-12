#import <string>
#import <vector>

class Registro{
protected:
	std::string* atr;
public:
	Registro();
	~Registro();
	
};

class Pagina {
protected:
	int tam, numReg;
	Registro* registros;	

public:
	Pagina();

	~Pagina();
	void inserirReg(Registro reg);
	bool cheia(){
		return numReg >= tam;
	}

};

class Tabela
{
protected:
	int numPag;
	std::vector <Pagina*> pages;
	std::string* esquema;
public:
	Tabela(string** novoEsquema){
		esquema = *novoEsquema;
	}
	~Tabela();


	
	//void addpag();
	void inserirTupla(string* novaTupla){
		if(testaEsquema(novaTupla)){

			Registro novoRegistro = new Registro(novaTupla);
			if (numPag == 0){
				pages.pushback(new Pagina(novoRegistro));
				numPag++;
			}
			else{
				if (!pages[numPag - 1].cheia()){
					page[numPag].inserirReg(novoRegistro);
				}
				else {
					pages.pushback(new Pagina(novoRegistro));
					numPag++;
					
				}
			}


		}
		
		return;

	}




};


class Operador : public Tabela
{
protected:
	
public:
	Operador(Tabela, string* projecao);
	~Operador();
	void extecuar;
	int numTuplasGeradas(); 	// GET soma dos registros das pags
	int numPagsGeradas();	  	// GET numPag
	string* tuplasGeradas() // GET resutaldo, transforma a table numa string unica
};