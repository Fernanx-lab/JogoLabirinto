#include<bits/stdc++.h>
using namespace std;

typedef struct {
    int baus;
    int curas;
    int bombas;
    int chaves;
}MapaItens;

typedef struct {
    vector<string> mapaDesenho;
    MapaItens itens;
}Mapa;

ifstream SelecaoDeMapa();
Mapa ObtemMapa();



ifstream SelecaoDeMapa()
{
    srand(time(NULL));
    int numeroDoMapa = rand() % 3;
    if(numeroDoMapa == 0)
    {
        numeroDoMapa = 1;
    }
    string nomeDoArquivo = "mapas/mapa " + to_string(numeroDoMapa) + ".txt";
    ifstream mapa;
    mapa.open(nomeDoArquivo);
    if(!mapa.is_open())
    {
        cout << "Erro ao abrir o arquivo do mapa!" << endl;
        exit(1);
    }
    return mapa;
}

Mapa ObtemMapa()
{
    Mapa mapa;
    ifstream arquivoDoMapa = SelecaoDeMapa();
    arquivoDoMapa.ignore(numeric_limits<streamsize>::max(), '\n');
    arquivoDoMapa.ignore(numeric_limits<streamsize>::max(), '\n');
    mapa.itens.baus = 0;
    
    while(!arquivoDoMapa.eof())
    {
        string linha;
        getline(arquivoDoMapa, linha);
        mapa.itens.baus += count(linha.begin(), linha.end(), 'B');
        mapa.mapaDesenho.push_back(linha);
    }
    return mapa;
}

void GerarItensMapa(Mapa& mapa)
{
    
}
