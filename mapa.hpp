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

inline vector<string> LerMapa()
{
    vector<string> mapa;
    ifstream arquivoDoMapa = SelecaoDeMapa();
    arquivoDoMapa.ignore(numeric_limits<streamsize>::max(), '\n');
    arquivoDoMapa.ignore(numeric_limits<streamsize>::max(), '\n');
    
    while(arquivoDoMapa.getline())
    {
        string linha;
        getline(arquivoDoMapa, linha);
        mapa.push_back(linha);
    }
    return mapa;
}
