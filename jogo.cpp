#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "personagem.hpp"
#include "jogo.hpp"

using namespace std;

static void imprimir_mapa_com_personagem(const Mapa& mapa, const Personagem& p) {
    vector<string> copia = mapa.mapaDesenho;
    if ((int)copia.size() == 0) return;
    int altura = (int)copia.size();
    int largura = (int)copia[0].size();
    if (p.linha >= 0 && p.linha < altura && p.coluna >= 0 && p.coluna < largura) {
        copia[p.linha][p.coluna] = '#';
    }

    cout << "Mapa (P = personagem)" << endl;

    // Cabeçalho de colunas com algarismos empilhados: centenas, dezenas, unidades
    cout << "    ";
    // Linha das centenas (most significant)
    for (int c = 0; c < largura; ++c) {
        int h = (c / 100) % 10;
        if (c >= 100) cout << h; else cout << ' ';
        cout << ' ';
    }
    cout << '\n';

    cout << "    ";
    // Linha das dezenas
    for (int c = 0; c < largura; ++c) {
        int d = (c / 10) % 10;
        if (c >= 10) cout << d; else cout << ' ';
        cout << ' ';
    }
    cout << '\n';

    cout << "    ";
    // Linha das unidades (least significant)
    for (int c = 0; c < largura; ++c) {
        int u = c % 10;
        cout << u << ' ';
    }
    cout << '\n';

    // Linhas com índice
    for (int i = 0; i < altura; ++i) {
        cout << setw(3) << i << ' ';
        for (int j = 0; j < largura; ++j) {
            char ch = copia[i][j];
            cout << ch << ' ';
        }
        cout << '\n';
    }
}

static void imprimir_inventario(const Personagem& p) {
    cout << "-- Inventario do personagem --\n";
    cout << "Nome: " << p.nome << "\n";
    cout << "Vida: " << p.vida_atual << " / " << p.vida_maxima << "\n";
    cout << "Chaves: " << p.qtd_chaves << "\n";
    cout << "Pocoes: " << p.qtd_pocoes << "\n";
    cout << "Bombas: " << p.qtd_bombas << "\n";
    if (p.qtd_pocoes > 0) {
        cout << "Detalhe das pocoes (cura): ";
        for (int i = 0; i < p.qtd_pocoes; ++i) {
            cout << p.pocoes[i].cura;
            if (i + 1 < p.qtd_pocoes) cout << ", ";
        }
        cout << "\n";
    }
    cout << "------------------------------\n";
}

static pair<int,int> encontra_primeira_celula_livre(const Mapa& mapa) {
    // Procura o caractere '#' no mapa — representa a posição inicial do jogador.
    for (int i = 0; i < (int)mapa.mapaDesenho.size(); ++i) {
        for (int j = 0; j < (int)mapa.mapaDesenho[i].size(); ++j) {
            char ch = mapa.mapaDesenho[i][j];
            if (ch == '#') return {i, j};
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

    // Marca a posição do jogador (onde havia '#') como célula livre no mapa
    // para que a BFS e renderização tratem a célula como caminhável.
    if (inicio.first >= 0) {
        mapa.mapaDesenho[inicio.first][inicio.second] = ' ';
    }

    Personagem jogador;
    personagem_inicializar(&jogador, "Jogador", inicio.first, inicio.second);

    cout << "Personagem inicializado em (" << jogador.linha << ", " << jogador.coluna << ")" << endl;

    while (true) {
        imprimir_mapa_com_personagem(mapa, jogador);
        cout << "Digite destino como 'linha coluna' (ou 'r' aleatorio, 'h' curar, 'q' sair): ";
        string linha;
        if (!std::getline(cin, linha)) break;
        if (linha.empty()) continue;
        if (linha == "q") break;
        else if (linha == "h") {
            if (jogador.qtd_pocoes == 0) {
                cout << "Nenhuma pocao disponivel." << endl;
            } else {
                cout << "Pocoes disponiveis:" << endl;
                for (int i = 0; i < jogador.qtd_pocoes; ++i) {
                    cout << i << ": Cura " << jogador.pocoes[i].cura << endl;
                }
                cout << "Digite o numero da pocao para usar: ";
                string slot_str;
                if (!std::getline(cin, slot_str)) break;
                int slot = -1;
                std::istringstream iss(slot_str);
                if (iss >> slot && personagem_usar_pocao(&jogador, slot)) {
                    cout << "Pocao usada! Vida atual: " << jogador.vida_atual << endl;
                } else {
                    cout << "Slot invalido." << endl;
                }
            }
        }
        int destL = -1, destC = -1;
        if (linha == "r") {
            vector<pair<int,int>> livres;
            for (int i = 0; i < (int)mapa.mapaDesenho.size(); ++i) {
                for (int j = 0; j < (int)mapa.mapaDesenho[i].size(); ++j) {
                    if (mapa.mapaDesenho[i][j] != '#') livres.push_back({i,j});
                }
            }
            if (livres.empty()) { cout << "Sem celulas livres." << endl; continue; }
                size_t nlivres = livres.size();
                auto sel = livres[rand() % nlivres];
            destL = sel.first; destC = sel.second;
            cout << "Destino aleatorio escolhido: (" << destL << ", " << destC << ")\n";
        } else {
            // parse
            std::istringstream iss(linha);
            if (!(iss >> destL >> destC)) {
                cout << "Entrada invalida." << endl; continue;
            }
        }

        bool vitoria = false;
        // Verifica se o destino é a saída
        if (mapa.mapaDesenho[destL][destC] == '@') {
            int chaves_necessarias = 3; // Com base em "$$$@"
            if (jogador.qtd_chaves >= chaves_necessarias) {
                for (int i = 0; i < chaves_necessarias; ++i) {
                    personagem_usar_chave(&jogador);
                }
                mapa.mapaDesenho[destL][destC] = ' ';
                vitoria = true;
            } else {
                cout << "Voce precisa de " << chaves_necessarias << " chaves para abrir a saida." << endl;
                continue;
            }
        }

        // Tenta navegar
        vector<pair<int,int>> caminho;
        bool ok = personagem_navegar_para_jogo(mapa, &jogador, destL, destC, &caminho);
        if (!ok) {
            cout << "Nao foi possivel encontrar caminho ate (" << destL << ", " << destC << ")." << endl;
        } else {
            cout << "Caminho encontrado (" << caminho.size() << " passos):\n";
            for (auto &s : caminho) cout << "(" << s.first << "," << s.second << ") ";
            cout << "\n";
            cout << "Personagem agora em (" << jogador.linha << ", " << jogador.coluna << ")" << endl;
        }
        // Imprime inventário do personagem após a escolha
        imprimir_inventario(jogador);
        if (vitoria) {
            cout << "Voce venceu!" << endl;
            break;
        }
    }

    cout << "Saindo..." << endl;
    return 0;
}
 