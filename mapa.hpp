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
    int altura;
    int largura;
}Mapa;

Mapa mapaInvisivel;

ifstream SelecaoDeMapa();
Mapa ObtemMapa();
void GerarItensMapa(Mapa* mapa);



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
    arquivoDoMapa >> mapa.largura >> mapa.altura;
    arquivoDoMapa.ignore(numeric_limits<streamsize>::max(), '\n');
    mapa.itens.baus = 0;

    while (!arquivoDoMapa.eof()) {
        string linha;
        getline(arquivoDoMapa, linha);
        mapa.itens.baus += count(linha.begin(), linha.end(), 'B');
        mapa.mapaDesenho.push_back(linha);
    }

    // Copia para o mapa invisível e gera itens com base no desenho real
    mapaInvisivel = mapa;
    GerarItensMapa(&mapa);
    GerarItensMapa(&mapaInvisivel);
    return mapa;
}

void GerarItensMapa(Mapa* mapa)
{
    int totalDeBaus = mapa->itens.baus;
    // Conta portas '$' no mapa — precisamos garantir chaves suficientes
    int totalDePortas = 0;
    for (const auto &linha : mapa->mapaDesenho) {
        totalDePortas += (int)count(linha.begin(), linha.end(), '$');
    }

    mapa->itens.curas = totalDeBaus / 3;
    mapa->itens.bombas = totalDeBaus / 4;
    int chavesPorBau = max(1, totalDeBaus / 5);
    // Garante ao menos uma chave por porta, ou chaves calculadas por baús, o que for maior
    mapa->itens.chaves = max(chavesPorBau, totalDePortas);
}
