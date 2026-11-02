# Algoritmos e Estruturas de Dados Avançadas (AEDS)
## Estudo Experimental Comparativo: Heap de Fibonacci e Árvore de van Emde Boas (vEB)

Projeto desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados Avançadas (AEDS)** do curso de **Engenharia de Computação**.

O repositório contém as implementações completas das estruturas de dados avançadas, suíte de testes de corretude, protocolo de experimentos com 10 repetições para análise empírica, scripts de geração gráfica e o **relatório experimental completo em padrão ABNT (26 páginas)**.

---

## 📌 Estruturas de Dados Implementadas

1. **Heap de Fibonacci** (`src/core/heap_fibonacci.cpp`, `src/include/heap_fibonacci.hpp`):
   - Operações: `insert`, `minimum`, `extract_min`, `decrease_key`, `delete_no`, `union_heaps`.
   - Política *lazy* com lista circular duplamente encadeada e consolidação em potências de $\phi$.
2. **Árvore de van Emde Boas (vEB)** (`src/core/veb.cpp`, `src/include/veb.hpp`):
   - Operações: `veb_insert`, `veb_delete`, `veb_member`, `veb_minimum`, `veb_maximum`, `veb_successor`, `veb_predecessor`.
   - Decomposição recursiva em universos de tamanho $u$, com sub-árvores *summary* e *clusters* de tamanho $\sqrt[\uparrow]{u}$ e $\sqrt[\downarrow]{u}$.
3. **Árvores Balanceadas de Busca (BST)**:
   - **Árvore AVL** (`src/include/avl.hpp`): balanceamento por fator de altura com rotações simples e duplas.
   - **Árvore Rubro-Negra** (`src/include/rb.hpp`): balanceamento por coloração e propriedades de caminho negro.
4. **Filas de Prioridade Alternativas e Grafos**:
   - **Heap D-Ário** (`src/core/heap_dario.cpp`, `src/include/heap_dario.hpp`): parametrização da aridade $d$.
   - **Algoritmo de Dijkstra** (`src/core/dijkstra.cpp`, `src/include/dijkstra.hpp`): execução sobre grafos direcionados esparsos e densos.

---

## 🚀 Como Compilar e Testar

O projeto foi desenvolvido em **C++20** e foi validado em **Linux (Ubuntu 24.04 LTS)** e **Windows 11**.

### Requisitos
- Compilador C++ com suporte a C++20 (`g++ >= 11`, `clang++ >= 13` ou `MSVC`).
- Python 3.8+ (opcional, apenas se desejar gerar novamente os gráficos; todos os gráficos já vêm pré-gerados em `results/`).

---

### Opção 1: Via Scripts de Automação (Mais Rápido e Recomendado)

#### No Linux / macOS:
```bash
# 1. Dar permissão de execução
chmod +x build.sh

# 2. Executar testes de corretude das estruturas (duração: ~1 segundo)
./build.sh test

# 3. Compilar todos os executáveis do projeto
./build.sh all
```

#### No Windows (Prompt de Comando ou PowerShell):
```cmd
:: 1. Executar testes de corretude das estruturas
build.bat test

:: 2. Compilar todos os executáveis do projeto
build.bat all
```

---

### Opção 2: Via Makefile (Linux)

```bash
# Executar testes unitários de corretude
make test

# Compilar todos os executáveis
make all

# Executar os benchmarks individualmente
make estudo1       # Filas de Prioridade (Fibonacci vs vEB)
make estudo2_c1    # Árvores de Busca (vEB vs AVL vs Rubro-Negra)
make estudo2_c2    # Dijkstra em Grafos Esparsos e Densos
```

---

### Opção 3: Compilação Manual Direta via `g++` (Sem scripts)

Caso prefira compilar diretamente linha a linha no terminal:

#### 1. Testes de Corretude (Valida 100% das operações das estruturas):
```bash
g++ -O3 -std=c++20 -I src/include tests/test_estruturas.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp src/core/dijkstra.cpp -o test_estruturas
./test_estruturas
```

#### 2. Benchmark do Estudo 1 (Heap de Fibonacci vs vEB):
```bash
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo1.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp -o benchmark_estudo1
./benchmark_estudo1
```

#### 3. Benchmark do Estudo 2 - Cenário 1 (vEB vs AVL vs Rubro-Negra):
```bash
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo2_cenario1.cpp src/core/veb.cpp -o benchmark_estudo2_cenario1
./benchmark_estudo2_cenario1
```

#### 4. Benchmark do Estudo 2 - Cenário 2 (Dijkstra com Fibonacci vs D-Ário):
```bash
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo2_cenario2.cpp src/core/dijkstra.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp -o benchmark_estudo2_cenario2
./benchmark_estudo2_cenario2
```

---

## 📊 Estrutura do Repositório

```text
├── data/                    # Bases de dados sintéticas (1.000.000 chaves e grafos)
│   ├── aleatorio.dat        # Chaves em ordem aleatória
│   ├── crescente.dat        # Chaves em ordem crescente
│   ├── decrescente.dat      # Chaves em ordem decrescente
│   ├── grafo_esparso.dat    # Grafo esparso (|V|=10.000, |E|=40.000)
│   └── grafo_denso.dat      # Grafo denso (|V|=10.000, |E|=10.000.000)
├── src/                     # Código-fonte das estruturas e benchmarks
│   ├── core/                # Implementações das estruturas (.cpp)
│   ├── include/             # Cabeçalhos e interfaces (.hpp)
│   ├── benchmark_estudo1.cpp
│   ├── benchmark_estudo2_cenario1.cpp
│   ├── benchmark_estudo2_cenario2.cpp
│   └── medir_memoria_valgrind.cpp
├── tests/                   # Suíte de testes unitários
│   └── test_estruturas.cpp  # Testes determinísticos com assert
├── results/                 # Resultados experimentais consolidados
│   ├── estudo1/             # Gráficos de tempo e memória do Estudo 1 (.png)
│   ├── estudo2/             # Gráficos de árvores e Dijkstra do Estudo 2 (.png)
│   ├── valgrind/            # Relatórios brutos de pico de memória com Massif
│   ├── *.pdf                # Gráficos vetoriais de alta resolução para o relatório
│   └── *.csv                # Dados tabulados com médias e desvios padrão (10 repetições)
├── report/                  # Relatório Experimental ABNT
│   ├── relatorio_experimental.pdf   # Documento final formatado (26 páginas)
│   └── relatorio_experimental.tex   # Código-fonte LaTeX compilável
├── scripts/                 # Scripts Python de geração dos gráficos
├── build.sh                 # Script de compilação e execução para Linux/macOS
├── build.bat                # Script de compilação e execução para Windows
└── Makefile                 # Makefile tradicional para ambientes Unix
```

---