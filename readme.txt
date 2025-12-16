README - JogoLabirinto

Resumo
- Projeto: protótipo de jogo de labirinto em C++.
- Objetivo desta atualização: implementar navegação automática por BFS, runner de teste interativo, inventário do personagem e interação com itens do mapa (baús, bombas, poções, chaves).

Arquivos principais (estado atual)
 - `jogo.hpp`:
   - Implementação inline (`static inline`) de BFS (`bfs_obter_caminho_jogo`) e função de navegação `personagem_navegar_para_jogo`.
   - Processamento de interações ao mover: ao entrar em uma célula com um baú (`'B'`) o baú é aberto e o jogador recebe um evento.
   - Regras do baú (loot): 1/3 de chance de chave, 1/3 de chance de poção (cura de 1 a 5), 1/3 de chance de bomba.
     - Quando sai bomba do baú, ela explode imediatamente no jogador causando dano aleatório entre 1 e 5 (não é adicionada ao inventário).
   - Considera `'*'` como parede (não caminhável) e `'#'` como representação do personagem no mapa.

 - `jogo.cpp`:
   - Runner interativo para testes.
   - Carrega mapa com `ObtemMapa()` (de `mapa.hpp`).
   - Localiza `'#'` no mapa como posição inicial do jogador; substitui `'#'` por espaço no `mapa.mapaDesenho` para tratar a célula como livre.
   - Mostra mapa numerado com índices de linha no lado esquerdo e colunas no topo (algarismos empilhados: centenas/dezenas/unidades).
   - Recebe entrada do usuário:
     - `linha coluna` — move até destino (se houver caminho).
     - `r` — escolhe destino aleatório não-parede.
     - `q` — sai do programa.
   - Após cada escolha, imprime o caminho encontrado (lista de coordenadas), posição final do personagem e inventário atual.

 - `personagem.hpp` (modificado):
   - Estrutura `Personagem` contém campos:
     - `nome` (char[TAM_NOME])
     - `vida_maxima`, `vida_atual`
     - `qtd_chaves`
     - `pocoes` (array) e `qtd_pocoes`
     - `qtd_bombas` (campo presente; atualmente não preenchido por baús porque bombas explodem ao serem sorteadas)
     - `linha`, `coluna`
   - Funções utilitárias inline: inicializar, adicionar/usar poção, receber dano, adicionar/usar chave, mover para, adicionar/usar bomba (estas últimas existem para uso futuro).

 - `mapa.hpp` (existente):
   - Lê mapas a partir de `mapas/mapa X.txt` e preenche `Mapa` com `mapaDesenho` (vector<string>) e contadores de itens.

Comportamento detalhado das interações
 - Paredes: qualquer célula `'*'` é considerada parede e não é caminhável.
 - Personagem inicial: localizado por `'#'` no arquivo do mapa. `jogo.cpp` remove esse caractere do desenho do mapa e inicializa `Personagem` nessa posição.
 - Baús (`'B'`): ao o personagem entrar na célula do baú (durante o movimento automático via BFS), o baú é processado imediatamente:
   - 1/3 de chance: chave — incrementa `p->qtd_chaves`.
   - 1/3 de chance: poção — adiciona uma poção com cura aleatória de 1..5 (`personagem_adicionar_pocao`).
   - 1/3 de chance: bomba — explode no jogador; chama `personagem_receber_dano(p, dano)` com dano aleatório 1..5. O baú é então removido (substituído por espaço).

Interface de terminal / exemplo de uso
1. Compilar (Windows, PowerShell) — task já configurada no workspace; ou direto:

   C:\TDM-GCC-64\bin\g++.exe -g jogo.cpp -o jogo.exe

2. Executar:

   .\jogo.exe

3. Interagir:
   - O mapa será exibido com índices de linha no lado esquerdo e colunas no topo (algarismos empilhados).
   - Digite `linha coluna` (ex.: `5 10`) para mover até essa célula.
   - Digite `r` para destino aleatório.
   - Digite `q` para sair.

Observações técnicas e decisões
 - Implementação das funções de navegação em header (`jogo.hpp`) utiliza `static inline` para evitar múltiplas definições em tempo de link e facilitar inclusão direta no runner.
 - A BFS retorna o caminho mais curto em termos de número de passos (4-direções) usando matriz `pai` para reconstrução.
 - A renderização do cabeçalho de colunas usa três linhas (centenas, dezenas, unidades) alinhadas com espaçamento mínimo para manter legibilidade em terminais.
 - Mensagens de abertura de baús e danos por bombas são impressas para feedback do jogador; a impressão de cada passo do movimento foi removida para reduzir ruído.

Estado atual / possíveis melhorias
 - A implementação funciona como protótipo de teste funcional. Melhorias sugeridas:
   - Implementar efeitos de bomba (abrir paredes adjacentes) em vez de dano apenas.
   - Animação passo-a-passo (desenhar mapa a cada passo com delay) para visualização do movimento.
   - Persistência de recordes em arquivo e tela de recordes real.
   - Reaproveitar `qtd_bombas` do `Personagem` (hoje o loot de bomba explode imediatamente; se quiser bombas no inventário, alterar comportamento dos baús).

Data: 2025-12-16
Atualizado por: integração de desenvolvimento no workspace
