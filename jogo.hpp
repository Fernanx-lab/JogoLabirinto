#ifndef JOGO_HPP
#define JOGO_HPP

// Jogo.hpp: reune funções de mapa, personagem e navegação (BFS)
// Implementações são inline/static para que `jogo.cpp` possa ser apenas o chamador.

#include "mapa.hpp"
#include "personagem.hpp"
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <iostream>
#include <cstdlib>

static inline bool dentro_limites_jogo(int l, int c, int altura, int largura) {
    return l >= 0 && l < altura && c >= 0 && c < largura;
}

// BFS para obter caminho mais curto (4-direções) entre duas células.
// Retorna vetor de pares (linha, coluna) incluindo início e destino.
static inline std::vector<std::pair<int,int>> bfs_obter_caminho_jogo(const Mapa& mapa,
                                                int startLinha, int startColuna,
                                                int goalLinha, int goalColuna)
{
    std::vector<std::pair<int,int>> vazio;

    int altura = (int)mapa.mapaDesenho.size();
    if (altura == 0) return vazio;
    int largura = (int)mapa.mapaDesenho[0].size();

    if (!dentro_limites_jogo(startLinha, startColuna, altura, largura) ||
        !dentro_limites_jogo(goalLinha, goalColuna, altura, largura)) {
        return vazio;
    }

    // Considera '*' como parede no formato dos mapas fornecidos; '#' é personagem.
    auto eh_parede = [&](int l, int c){ return mapa.mapaDesenho[l][c] == '*'; };

    if (eh_parede(startLinha, startColuna) || eh_parede(goalLinha, goalColuna)) {
        return vazio;
    }

    std::vector<std::vector<bool>> visitado(altura, std::vector<bool>(largura, false));
    std::vector<std::vector<std::pair<int,int>>> pai(altura, std::vector<std::pair<int,int>>(largura, {-1,-1}));

    std::queue<std::pair<int,int>> q;
    q.push({startLinha, startColuna});
    visitado[startLinha][startColuna] = true;

    const int dL[4] = {-1, 0, 1, 0};
    const int dC[4] = {0, 1, 0, -1};

    bool achou = false;
    while(!q.empty()) {
        auto cur = q.front(); q.pop();
        int l = cur.first, c = cur.second;
        if (l == goalLinha && c == goalColuna) { achou = true; break; }

        for (int i = 0; i < 4; ++i) {
            int nl = l + dL[i];
            int nc = c + dC[i];
            if (!dentro_limites_jogo(nl, nc, altura, largura)) continue;
            if (visitado[nl][nc]) continue;
            if (eh_parede(nl, nc)) continue;
            visitado[nl][nc] = true;
            pai[nl][nc] = {l, c};
            q.push({nl, nc});
        }
    }

    if (!achou) return vazio;

    std::vector<std::pair<int,int>> caminho;
    std::pair<int,int> cur = {goalLinha, goalColuna};
    while (!(cur.first == -1 && cur.second == -1)) {
        caminho.push_back(cur);
        if (cur.first == startLinha && cur.second == startColuna) break;
        cur = pai[cur.first][cur.second];
    }

    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

// Move o personagem ao longo do caminho mais curto até destino (se existir).
// Retorna true se o movimento foi realizado; preenche out_path se fornecido.
static inline bool personagem_navegar_para_jogo(Mapa& mapa, Personagem* p, int goalLinha, int goalColuna,
                             std::vector<std::pair<int,int>>* out_path = nullptr)
{
    if (!p) return false;

    auto caminho = bfs_obter_caminho_jogo(mapa, p->linha, p->coluna, goalLinha, goalColuna);
    if (caminho.empty()) {
        return false;
    }

    if (out_path) *out_path = caminho;

    for (size_t i = 1; i < caminho.size(); ++i) {
        int nl = caminho[i].first;
        int nc = caminho[i].second;
        personagem_mover_para(p, nl, nc);

        // Processa interações na célula (ex.: baú 'B')
        if (mapa.mapaDesenho[nl][nc] == 'B') {
            // Abrir baú: 1/3 chave, 1/3 poção (cura 1..5), 1/3 bomba
            int escolha = rand() % 3;
            if (escolha == 0) {
                personagem_adicionar_chave(p);
                std::cout << "Baú aberto em (" << nl << "," << nc << "): chave obtida." << std::endl;
            } else if (escolha == 1) {
                int cura = (rand() % 5) + 1;
                personagem_adicionar_pocao(p, cura);
                std::cout << "Baú aberto em (" << nl << "," << nc << "): poção (cura " << cura << ") obtida." << std::endl;
            } else {
                // Bomba explode na cara do jogador: dano aleatório 1..5
                int dano = (rand() % 5) + 1;
                personagem_receber_dano(p, dano);
                std::cout << "Baú aberto em (" << nl << "," << nc << "): bomba explodiu! "
                          << "Dano recebido: " << dano << "." << std::endl;
            }
            // Marca o baú como aberto
            mapa.mapaDesenho[nl][nc] = ' ';
        }
    }

    return true;
}

// Stubs simples para telas — jogo.cpp deve implementar a UI real.
static inline void mostrar_tela_inicial_stub() {
    std::cout << "[Tela inicial] Pressione ENTER para iniciar...\n";
}

static inline void mostrar_tela_recordes_stub() {
    std::cout << "[Tela de recordes] (implementacao futura)\n";
}

#endif // JOGO_HPP
