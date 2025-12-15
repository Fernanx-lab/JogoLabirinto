#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "personagem.hpp"
#include "jogo.hpp"

using namespace std;

static void imprimir_mapa_com_personagem(const Mapa& mapa, const Personagem& p) {
    vector<string> copia = mapa.mapaDesenho;
    if ((int)copia.size() == 0) return;
    int altura = (int)copia.size();
    int largura = (int)copia[0].size();
    if (p.linha >= 0 && p.linha < altura && p.coluna >= 0 && p.coluna < largura) {
        copia[p.linha][p.coluna] = 'P';
    }
    cout << "Mapa (P = personagem)" << endl;
    for (const auto& linha : copia) {
        cout << linha << '\n';
    }
}

static pair<int,int> encontra_primeira_celula_livre(const Mapa& mapa) {
    for (int i = 0; i < (int)mapa.mapaDesenho.size(); ++i) {
        for (int j = 0; j < (int)mapa.mapaDesenho[i].size(); ++j) {
            char ch = mapa.mapaDesenho[i][j];
            if (ch != '#') return {i, j};
        }
    }
    return {-1, -1};
}

int main() {
    srand((unsigned)time(NULL));

    cout << "Carregando mapa..." << endl;
    Mapa mapa = ObtemMapa();

    auto inicio = encontra_primeira_celula_livre(mapa);
    if (inicio.first == -1) {
        cerr << "Nenhuma célula livre encontrada no mapa." << endl;
        return 1;
    }

    Personagem jogador;
    personagem_inicializar(&jogador, "Jogador", inicio.first, inicio.second);

    cout << "Personagem inicializado em (" << jogador.linha << ", " << jogador.coluna << ")" << endl;

    while (true) {
        imprimir_mapa_com_personagem(mapa, jogador);
        cout << "Digite destino como 'linha coluna' (ou 'r' aleatorio, 'q' sair): ";
        string linha;
        if (!std::getline(cin, linha)) break;
        if (linha.empty()) continue;
        if (linha == "q") break;
        int destL = -1, destC = -1;
        if (linha == "r") {
            // escolhe uma célula aleatória não-parede
            vector<pair<int,int>> livres;
            for (int i = 0; i < (int)mapa.mapaDesenho.size(); ++i) {
                for (int j = 0; j < (int)mapa.mapaDesenho[i].size(); ++j) {
                    if (mapa.mapaDesenho[i][j] != '#') livres.push_back({i,j});
                }
            }
            if (livres.empty()) { cout << "Sem células livres." << endl; continue; }
                size_t nlivres = livres.size();
                auto sel = livres[rand() % nlivres];
            destL = sel.first; destC = sel.second;
            cout << "Destino aleatório escolhido: (" << destL << ", " << destC << ")\n";
        } else {
            // parse
            std::istringstream iss(linha);
            if (!(iss >> destL >> destC)) {
                cout << "Entrada inválida." << endl; continue;
            }
        }

        // Tenta navegar
        vector<pair<int,int>> caminho;
        bool ok = personagem_navegar_para_jogo(mapa, &jogador, destL, destC, &caminho);
        if (!ok) {
            cout << "Não foi possível encontrar caminho até (" << destL << ", " << destC << ")." << endl;
        } else {
            cout << "Caminho encontrado (" << caminho.size() << " passos):\n";
            for (auto &s : caminho) cout << "(" << s.first << "," << s.second << ") ";
            cout << "\n";
            cout << "Personagem agora em (" << jogador.linha << ", " << jogador.coluna << ")" << endl;
        }
    }

    cout << "Saindo..." << endl;
    return 0;
}
 