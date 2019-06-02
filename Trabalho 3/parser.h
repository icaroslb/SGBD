#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

enum class Type {BT, R, W, CM};
typedef struct Operation{
    int  id, item;
    Type ope;
    char data;
}OP;


void abrirArquivo (std::fstream &arquivo);
int lerNum (std::string &texto, int &pos);
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

int lerNum (std::string &texto, int &pos) {
    int init = pos, tam = 0;

    while(texto[pos] >= '0' && texto[pos] <= '9'){
        pos++;
        tam++;
    }
    std::cout << std::stoi(texto.substr(init, tam), nullptr) << '\n';
    return std::stoi(texto.substr(init, tam), nullptr);
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
        
        for(int i = 0; i < tam; i++){
            if(texto.compare(i, 2, "BT") == 0){
                opLida.ope = Type::BT;
                i += 3;
                opLida.id = lerNum(texto, i);
                opLida.item = -1;
                opLida.data = -1;
            }else if(texto.compare(i, 1, "R") == 0){
                opLida.ope = Type::R;
                i++;
                opLida.id = lerNum(texto, i);

                i++;
                if(pag.find(texto[i]) == pag.end()){
                    pag[texto[i]] = idPagina++;
                }

                opLida.item = pag[texto[i]];
                opLida.data = texto[i];

                i++;
            }else if(texto.compare(i, 1, "W") == 0){
                opLida.ope = Type::W;
                i++;
                opLida.id = lerNum(texto, i);
                
                i++;
                if(pag.find(texto[i]) == pag.end()){
                    pag[texto[i]] = idPagina++;
                }

                opLida.item = pag[texto[i]];
                opLida.data = texto[i];

                i++;
            }else if(texto.compare(i, 2, "CM") == 0){
                opLida.ope = Type::CM;
                i += 3;
                opLida.id = lerNum(texto, i);
                opLida.item = -1;
                opLida.data = -1;
            }
        
            historia->push_back(opLida);
        }
        
        linhas->push_back(historia);

        historia = new std::vector <OP>;
    }

    arquivo.close();
    return linhas;
}

#endif //PARSER_H