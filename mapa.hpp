#include<bits/stdc++.h>
#include<format>
using namespace std;

char mapa[10][10]; 

ifstream SelecaoDeMapa();




ifstream SelecaoDeMapa()
{
    srand(time(NULL));
    int numeroDoMapa = rand() % 3;
    string nomeDoArquivo = format("mapa {0}.txt", numeroDoMapa); 
    ifstream mapa;
    mapa.open(nomeDoArquivo);
    return mapa;
}