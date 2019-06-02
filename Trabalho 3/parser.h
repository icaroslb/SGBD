#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

enum class Type {BT, R, W, CM};
typedef struct Operation{
    int  id;
    Type ope;
    char item;
}OP;


void abrirArquivo (std::fstream &arquivo);
std::vector < std::vector <OP>* >* parser ();


void abrirArquivo (std::fstream &arquivo) {
    std::string nomeArquivo;

    do{
        std::cout << "Nome do arquivo: ";
        std::cin >> nomeArquivo;

        arquivo.open(nomeArquivo, std::fstream::in);

        if(!(arquivo.is_open())){
            std::cout << "Não foi possível abrir o arquivo " << nomeArquivo << "!\n\n";
        }
    }while(!(arquivo.is_open()));
}


std::vector < std::vector <OP>* >* parser () {
    std::fstream arquivo;
    std::vector < std::vector <OP>* > *linhas = new std::vector < std::vector <OP> * >;
    std::vector <OP> *historia = new std::vector <OP>;
    std::map<char, int> pag;
    std::string texto;
    OP opLida;
    int tam, idPagina = 0;;

    abrirArquivo(arquivo);
    
    while(arquivo.good()){
        std::getline(arquivo, texto);
        tam = texto.size();
        
        for(int i = 0; i < tam; i += 6){
            if(texto.compare(i, 2, "BT") == 0){
                opLida.ope = Type::BT;
                opLida.id = texto[i+3];
                opLida.item = -1;
            }else if(texto.compare(i, 1, "R") == 0){
                opLida.ope = Type::R;
                opLida.id = texto[i+1];

                if(pag.find(texto[i+3]) == pag.end()){
                    pag[texto[i+3]] = idPagina++;
                }

                opLida.item = pag[texto[i+3]];
            }else if(texto.compare(i, 1, "W") == 0){
                opLida.ope = Type::W;
                opLida.id = texto[i+1];
                
                if(pag.find(texto[i+3]) == pag.end()){
                    pag[texto[i+3]] = idPagina++;
                }

                opLida.item = pag[texto[i+3]];
            }else if(texto.compare(i, 2, "CM") == 0){
                opLida.ope = Type::CM;
                opLida.id = texto[i+3];
                opLida.item = -1;
            }
            opLida.id -= '0';
        
            historia->push_back(opLida);
        }
        
        linhas->push_back(historia);

        historia = new std::vector <OP>;
    }

    arquivo.close();
    return linhas;
}

#endif //PARSER_H