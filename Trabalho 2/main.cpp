#include <iostream>
#include <string>
#include "headers/Tabela.h"
#include "headers/Pagina.h"
#include "headers/Registro.h"
#include "headers/Projecao.h"

// VOCÊ DEVE INCLUIR OS SEUS HEADERS AQUI!!!

int main() {
    std::vector<std::string> vinho_cols {"vinho_id", "rotulo", "ano_colheita", "pais_producao_id", "uva_id"};
    std::vector<std::string> uva_cols   {"uva_id", "nome", "tipo", "pais_origem_id"};
    std::vector<std::string> pais_cols  {"pais_id", "nome"};

    Tabela vinho {vinho_cols};
    Tabela uva   {uva_cols};
    Tabela pais  {pais_cols};

    pais.inserirTupla(new std::string[2] {"0", "Brasil"});
   	pais.inserirTupla(new std::string[2] {"1", "Franca"});
    pais.inserirTupla(new std::string[2] {"2", "Italia"});

    uva.inserirTupla(new std::string[4] {"0", "uva0", "tinto",  "0"});
    uva.inserirTupla(new std::string[4] {"1", "uva1", "branco", "1"});
    uva.inserirTupla(new std::string[4] {"2", "uva2", "tinto",  "2"});

    vinho.inserirTupla(new std::string[5] {"0", "vinho0", "1990", "0", "0"});
    vinho.inserirTupla(new std::string[5] {"1", "vinho1", "1991", "1", "0"});
    vinho.inserirTupla(new std::string[5] {"2", "vinho2", "1992", "2", "2"});



    // DESCOMENTE O TIPO DO OPERADOR QUE VOCE IRA IMPLEMENTAR!!!

//    // SELECAO:
//    Operador op {vinho, "rotulo", "uva0"}; // representa SELECT * FROM Vinho WHERE rotulo=uva0

//    // PROJECAO:
    std::vector<std::string> projecao_cols {"vinho_id", "uva_id"};
    Operador op {vinho, projecao_cols}; // representa SELECT vinho_id, uva_id FROM Vinho

//    // JUNCAO:
//    Operador op {vinho, uva, "vinho_id", "uva_id"}; // representa SELECT * FROM Vinho, Uva WHERE vinho_id=uva_id



    op.executar(); // Realiza a operacao desejada.

    int num_tuplas  = op.numTuplasGeradas(); // Retorna a quantidade de tuplas geradas pela operacao.
    int num_pags    = op.numPagsGeradas();  // Retorna a quantidade de paginas geradas pela operacao.

    std::vector<std::string*> tuplas = op.tuplasGeradas(); // Retorna as tuplas geradas pela operacao.
    
    printf("Numero tuplas: %d\nNumero páginas: %d\n\n", num_tuplas, num_pags);
    for(std::string *i : tuplas){
    	std::cout << i[0] << " - " << i[1] << std::endl;
    }
    
    return 0;
}
