all: Registro.o Pagina.o Tabela.o Projecao.o main

Registro.o: headers/Registro.h codes/Registro.cpp
	g++ -g -c codes/Registro.cpp

Pagina.o: headers/Pagina.h codes/Pagina.cpp
	g++ -g -c codes/Pagina.cpp

Tabela.o: headers/Tabela.h codes/Tabela.cpp
	g++ -g -c codes/Tabela.cpp

Projecao.o: headers/Projecao.h codes/Projecao.cpp
	g++ -g -c codes/Projecao.cpp

main: Registro.o Pagina.o Tabela.o Projecao.o main.cpp
	g++ main.cpp Projecao.o Tabela.o Pagina.o Registro.o -o main

clean:
	rm *.o
