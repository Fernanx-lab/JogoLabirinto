#include "navegacao.hpp"
#include <queue>
#include <iostream>

static inline bool dentro_limites(int l, int c, int altura, int largura) {
    return l >= 0 && l < altura && c >= 0 && c < largura;
}

std::vector<std::pair<int,int>> bfs_obter_caminho(const Mapa& mapa,
                                                int startLinha, int startColuna,
                                                int goalLinha, int goalColuna)
{
    std::vector<std::pair<int,int>> vazio;

    int altura = (int)mapa.mapaDesenho.size();
    if (altura == 0) return vazio;
    int largura = (int)mapa.mapaDesenho[0].size();

    if (!dentro_limites(startLinha, startColuna, altura, largura) ||
        !dentro_limites(goalLinha, goalColuna, altura, largura)) {
        return vazio;
    }

    // Caracteres que representam parede — pressuposição: '#' é parede.
    auto eh_parede = [&](int l, int c){ return mapa.mapaDesenho[l][c] == '#'; };

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
            if (!dentro_limites(nl, nc, altura, largura)) continue;
            if (visitado[nl][nc]) continue;
            if (eh_parede(nl, nc)) continue;
            visitado[nl][nc] = true;
            pai[nl][nc] = {l, c};
            q.push({nl, nc});
        }
    }

    if (!achou) return vazio;

    // Reconstruir caminho do destino até o início
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

bool personagem_navegar_para(Mapa& mapa, Personagem* p, int goalLinha, int goalColuna,
                             std::vector<std::pair<int,int>>* out_path)
{
    if (!p) return false;

    auto caminho = bfs_obter_caminho(mapa, p->linha, p->coluna, goalLinha, goalColuna);
    if (caminho.empty()) {
        return false;
    }

    // Opcional: preencher out_path
    if (out_path) *out_path = caminho;

    // O primeiro elemento do caminho é a posição atual; saltamos para os movimentos seguintes
    for (size_t i = 1; i < caminho.size(); ++i) {
        int nl = caminho[i].first;
        int nc = caminho[i].second;
        // Atualiza posição do personagem
        personagem_mover_para(p, nl, nc);
        // Atualiza informação no mapa se necessário (não modifiquei o mapa aqui)
        // Em jogos reais, aqui poderiam ser tratadas colisões, itens, inimigos etc.
    }

    return true;
}
