#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>
#include <typeinfo>

template <typename T>

class hash{
    public:
        hash();
        void inserir(T objeto);
        virtual ~hash();

    protected:

    private:
        unsigned int comp;
        T **tabela;
        
        unsigned int funcaoHash(T valor);
};

#endif // HASH_H
