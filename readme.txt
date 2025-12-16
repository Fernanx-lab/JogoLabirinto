README - JogoLabirinto

Descrição
- Este projeto é um protótipo de um jogo de labirinto em C++.
- Foi adicionada navegação por BFS (busca em largura) para mover um personagem (`Personagem`) pelo mapa na menor distância (4-direções).
- A navegação e utilitários foram consolidados em `jogo.hpp` como funções `static inline` para seguir o padrão do repositório.

Arquivos principais criados/alterados
- jogo.hpp
  - Contém a implementação inline do BFS (`bfs_obter_caminho_jogo`) e da função de navegação (`personagem_navegar_para_jogo`).
  - Contém stubs para telas (tela inicial, recordes) para uso futuro.
  - Removido o print de cada movimento para evitar saída excessiva durante a movimentação.
  - Pressupostos: paredes são representadas por `#` (caractere não caminhável).

- jogo.cpp
  - Runner interativo para testar o jogo:
    - Carrega um mapa usando `ObtemMapa()` (implementado em `mapa.hpp`).
    - Inicializa o personagem (`Personagem`) na primeira célula livre encontrada.
    - Exibe o mapa numerado (linhas e colunas) — o usuário pode escolher destino usando índices numéricos.
    - Entrada do usuário:
      - Digitar `linha coluna` para mover o personagem até o destino (se houver caminho).
      - Digitar `r` para escolher um destino aleatório não-parede.
      - Digitar `q` para sair.
    - Ao encontrar um caminho, o personagem é movido até o destino (posição atual atualizada) e o caminho é exibido como sequência de pares `(linha,coluna)`.

- personagem.hpp (existente)
  - Estrutura `Personagem` e utilitários (inicializar, mover, usar poções, chaves, etc.).

- mapa.hpp (existente)
  - Estrutura `Mapa`, leitura do arquivo de mapas em `mapas/mapa X.txt` e geração de itens.

Alterações técnicas importantes
- Todas as implementações de navegação estão em `jogo.hpp` como `static inline` para evitar problemas de múltipla definição ao incluir o header em múltiplos módulos.
- A BFS explora 4 direções (N,E,S,O) e retorna o caminho mais curto reconstruído via matriz `pai`.
- Removi `navegacao.hpp` e `navegacao.cpp` para evitar duplicação (código consolidado em `jogo.hpp`).

Como compilar e executar
- No Windows (PowerShell), usando a task existente ou diretamente com o g++ do TDM-GCC (conforme o ambiente do workspace):

  # Compilar
  C:\TDM-GCC-64\bin\g++.exe -g jogo.cpp -o jogo.exe

  # Executar
  .\jogo.exe

- Ou use a task do VS Code já configurada (Menu Run/Tasks -> `C/C++: g++.exe build active file`).

Assunções e recomendações
- Mapas devem ser retangulares (mesma largura em todas as linhas). Caso contrário, o renderer atual pode acessar posições inválidas.
- Símbolo `#` é tratado como parede. Se usar outro símbolo, altere a checagem em `jogo.hpp` (lambda `eh_parede`).
- Atualmente a movimentação não trata coleta de itens, inimigos ou portas. Para implementar isso, estenda o laço em `personagem_navegar_para_jogo` ou processe a célula após cada passo em `jogo.cpp`.
- Para animação passo-a-passo, adicione delays entre movimentos (ex.: `std::this_thread::sleep_for`) e renderize o mapa a cada passo.

Notas finais
- Se desejar, posso:
  - Adicionar lógica de interação com o mapa (baús, chaves, curas) durante a movimentação.
  - Implementar uma UI textual mais rica (limpar tela, atualizar posição dinamicamente).
  - Salvar/ler recordes (arquivo) e implementar a tela de recordes.

Feito por: ferramenta de auxílio ao desenvolvimento (integração no repositório)
Data: 2025-12-16
