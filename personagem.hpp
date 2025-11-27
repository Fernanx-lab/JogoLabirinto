#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TAM_NOME       11      // Nome de até 10 caracteres + '\0'
#define MAX_POCOES     50      // Limite de slots de poções

/* ============================================================
 *                       ESTRUTURAS
 * ============================================================ */

// Estrutura de uma poção individual
typedef struct {
    int cura;   // valor da cura entre 1 e 5
} Pocao;

// Estrutura principal do personagem
typedef struct {
    char nome[TAM_NOME];

    int vida_maxima;  
    int vida_atual;

    int qtd_chaves;

    Pocao pocoes[MAX_POCOES];
    int qtd_pocoes;

    int linha;
    int coluna;

} Personagem;


/* ============================================================
 *                    FUNÇÕES DO PERSONAGEM
 * ============================================================ */

// Inicializa o personagem
static inline void personagem_inicializar(Personagem *p, const char *nome, int linha_ini, int coluna_ini) {
    strncpy(p->nome, nome, TAM_NOME - 1);
    p->nome[TAM_NOME - 1] = '\0';

    p->vida_maxima = 10;
    p->vida_atual = 10;

    p->qtd_chaves = 0;
    p->qtd_pocoes = 0;

    p->linha = linha_ini;
    p->coluna = coluna_ini;
}

// Adiciona uma poção ao inventário
static inline bool personagem_adicionar_pocao(Personagem *p, int valor_cura) {
    if (p->qtd_pocoes >= MAX_POCOES) {
        return false;
    }

    p->pocoes[p->qtd_pocoes].cura = valor_cura;
    p->qtd_pocoes++;
    return true;
}

// Usa uma poção em um slot específico
static inline bool personagem_usar_pocao(Personagem *p, int slot) {
    if (slot < 0 || slot >= p->qtd_pocoes) {
        return false;
    }

    int cura = p->pocoes[slot].cura;

    p->vida_atual += cura;
    if (p->vida_atual > p->vida_maxima) {
        p->vida_atual = p->vida_maxima;
    }

    // Remover a poção deslocando as demais
    for (int i = slot; i < p->qtd_pocoes - 1; i++) {
        p->pocoes[i] = p->pocoes[i + 1];
    }

    p->qtd_pocoes--;

    return true;
}

// Aplica dano ao personagem
static inline void personagem_receber_dano(Personagem *p, int dano) {
    p->vida_atual -= dano;
    if (p->vida_atual < 0) {
        p->vida_atual = 0;
    }
}

// Verifica se personagem está vivo
static inline bool personagem_esta_vivo(const Personagem *p) {
    return p->vida_atual > 0;
}

// Adiciona uma chave
static inline void personagem_adicionar_chave(Personagem *p) {
    p->qtd_chaves++;
}

// Usa uma chave
static inline bool personagem_usar_chave(Personagem *p) {
    if (p->qtd_chaves <= 0) {
        return false;
    }

    p->qtd_chaves--;
    return true;
}

// Move o personagem para uma nova posição
static inline void personagem_mover_para(Personagem *p, int nova_linha, int nova_coluna) {
    p->linha = nova_linha;
    p->coluna = nova_coluna;
}


#endif // PERSONAGEM_H