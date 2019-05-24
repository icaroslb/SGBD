#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum {BT, R, W, CM};

struct operacao {
    int ope, id;
    char item;
};

void abrirArquivo (std::fstream &arquivo);
std::vector < std::vector <operacao> > parser ();


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


std::vector < std::vector <operacao> > parser () {
    std::fstream arquivo;
    std::vector < std::vector <operacao> > linhas;
    std::vector <operacao> historia;
    std::string texto;
    operacao opLida;
    int tam;

    abrirArquivo(arquivo);
    
    while(arquivo.good()){
        std::getline(arquivo, texto);
        tam = texto.size();
        
        for(int i = 0; i < tam; i += 6){
            if(texto.compare(i, 2, "BT") == 0){
                opLida.ope = BT;
                opLida.id = texto[i+3];
                opLida.item = -1;
            }else if(texto.compare(i, 1, "R") == 0){
                opLida.ope = R;
                opLida.id = texto[i+1];
                opLida.item = texto[i+3];
            }else if(texto.compare(i, 1, "W") == 0){
                opLida.ope = W;
                opLida.id = texto[i+1];
                opLida.item = texto[i+3];
            }else if(texto.compare(i, 2, "CM") == 0){
                opLida.ope = CM;
                opLida.id = texto[i+3];
                opLida.item = -1;
            }
            opLida.id -= '0';

            historia.push_back(opLida);
        }
        
        linhas.push_back(historia);

        historia.clear();
    }

    arquivo.close();
    return linhas;
}

#endif //PARSER_H