# Benchmarks — Árvore de Van Emde Boas (vEB Tree)

## Objetivo

A suíte de benchmarks avalia quantitativamente a eficiência temporal e o consumo de memória da **Árvore de Van Emde Boas (*vEB Tree*)** frente a estruturas de dados de referência da biblioteca padrão C++ (`std::set` e `std::vector<bool>` / `std::bitset`). O objetivo central é confrontar as garantias assintóticas teóricas — notadamente a independência de $N$ e o custo $O(\log \log u)$ — com o comportamento real do processador (efeitos de hierarquia de memória, *cache misses* e *branch mispredictions*).

## Organização dos Benchmarks

```text
tests/
└── benchmark_van_emde_boas.cpp  # Executor C++ de alta precisão com medição de memória
benchmarks/
├── data/                         # Saídas textuais tabulares (.txt)
│   ├── veb_tempo_insercao.txt
│   ├── veb_tempo_busca.txt
│   ├── veb_tempo_sucessor.txt
│   ├── veb_tempo_remocao.txt
│   └── veb_consumo_memoria.txt
├── figures/                      # Gráficos renderizados (.pdf e .png)
│   ├── tempo_vs_universo.pdf
│   ├── tempo_vs_densidade.pdf
│   ├── sucessor_comparativo.pdf
│   └── memoria_vs_universo.pdf
├── tables/                       # Tabelas consolidadas em LaTeX
│   └── tabela_desempenho_veb.tex
├── gerar_graficos.py             # Script Python para plotagem com matplotlib
└── analise_graficos.ipynb        # Notebook Jupyter para exploração estatística
```

## Protocolo Experimental e Parâmetros de Entrada

Para mitigar ruídos térmicos e variações no escalonador do sistema operacional:

1. **Repetições:** Cada cenário experimental é executado rigorosamente com **10 repetições independentes ($10\times$)**, computando média aritmética ($\mu$), desvio padrão ($\sigma$) e intervalo de confiança a $95\%$.
2. **Isolamento de Entrada:** Todas as chaves pseudoaleatórias são pré-alocadas em vetores contíguos (`std::vector<uint64_t>`) antes do início da contagem de tempo, garantindo que o custo do gerador (`std::mt19937_64`) não contamine a medição das estruturas.
3. **Relógio de Precisão:** O tempo é aferido por `std::chrono::high_resolution_clock`, calculando o tempo médio amortizado por operação em nanossegundos ($ns/op$) e o tempo total em microssegundos ($\mu s$).
4. **Descarte de Warm-up:** Uma rodada preliminar não contabilizada é executada para aquecimento de caches L1/L2/L3 e paginação de memória.

### Dimensões Experimentais

- **Escala do Universo ($u$):**
  $$u \in \{2^8 = 256,\ 2^{12} = 4096,\ 2^{16} = 65.536,\ 2^{20} = 1.048.576,\ 2^{24} = 16.777.216\}$$
- **Densidade de Preenchimento ($\alpha = N / u$):**
  - *Ultra-esparsa:* $\alpha = 0{,}001$ ($0{,}1\%$ de preenchimento);
  - *Esparsa:* $\alpha = 0{,}01$ ($1\%$ de preenchimento);
  - *Moderada:* $\alpha = 0{,}10$ ($10\%$ de preenchimento);
  - *Densa:* $\alpha = 0{,}50$ ($50\%$ de preenchimento);
  - *Saturada:* $\alpha = 0{,}90$ ($90\%$ de preenchimento).
- **Distribuições de Chaves:**
  - Uniforme pseudoaleatória;
  - Sequencial contígua ($0, 1, 2, \dots, N-1$);
  - Agrupada em *clusters* locais (simulando acessos com alta localidade espacial).

## Estruturas de Dados Concorrentes (Baselines)

1. **`vEB Tree` (Implementação Procedural do Projeto):**
   - Busca, sucessor, predecessor, inserção e remoção em $O(\log \log u)$.
   - Extremos em $O(1)$.
2. **`std::set<uint64_t>` (Árvore Rubro-Negra da STL):**
   - Operações dinâmicas e sucessor (`std::upper_bound`) em $O(\log N)$.
   - Overhead de nós de $32$ a $48$ bytes por elemento.
3. **`std::vector<bool>` / `std::bitset` (Bitmap Denso):**
   - Inserção, remoção e teste de pertinência em $O(1)$ estrito.
   - Sucessor e predecessor via varredura linear de bits ($O(u)$) ou com instruções intrinsics de hardware (`_tzcnt_u64` / `__builtin_ctzll`) atingindo $O(u/64)$.
   - Consumo de memória compacto e fixo de $u/8$ bytes.

## Operações Monitoradas

1. **Inserção em Massa:** Carga incremental de $N$ chaves até atingir a densidade $\alpha$.
2. **Pertinência (`member`):** Amostragem de $N$ consultas uniformes (50% de chaves presentes e 50% ausentes).
3. **Sucessor Ordenado (`successor`):** $N$ consultas sucessivas para chaves aleatórias $x \in [0, u - 1]$. Confronta a busca em $O(\log \log u)$ da vEB com o `upper_bound` ($O(\log N)$) do `std::set` e a varredura do bitset.
4. **Predecessor Ordenado (`predecessor`):** $N$ consultas consecutivas explorando o limite inferior.
5. **Remoção em Massa (`delete`):** Desalocação sequencial de todas as chaves previamente inseridas.

## Modelagem e Medição de Consumo de Memória

A análise espacial contempla duas métricas complementares:

1. **Memória Teórica Modelada:**
   - Para a vEB Eager: $S(u) = (\sqrt{u} + 1) S(\sqrt{u}) + O(\sqrt{u}) = O(u)$ bytes.
   - Para a vEB Lazy: $S(N, u) = O(N \log \log u)$ bytes.
   - Para `std::set`: $N \times (3 \times \text{sizeof(void*)} + \text{sizeof(T)} + \text{overhead de alinhamento}) \approx 48 N$ bytes.
   - Para `std::vector<bool>` / `bitset`: $\lceil u / 8 \rceil$ bytes.
2. **Memória Empírica do Processo:**
   - Capturada antes e depois da instanciação da estrutura através de APIs nativas do sistema:
     - Windows: `GetProcessMemoryInfo` inspecionando `WorkingSetSize` e `PrivateUsage`.
     - Linux/POSIX: Leitura de `/proc/self/status` dos campos `VmRSS` e `VmSize`.

## Formato dos Registros Brutos (.txt)

Os dados são exportados em formato tabular delimitado por espaço em `benchmarks/data/`:

```text
# estrutura universo_u densidade_alpha entrada_n repeticao tempo_us ns_por_op
veb_tree 65536 0.100 6553 1 128.40 19.59
veb_tree 65536 0.100 6553 2 125.10 19.09
std_set 65536 0.100 6553 1 342.80 52.31
bitset 65536 0.100 6553 1 89.20 13.61
...
```

E para medição de memória:

```text
# estrutura universo_u entrada_n repeticao memoria_teorica_bytes working_set_kb
veb_tree 65536 6553 1 1048576 4210
std_set 65536 6553 1 314544 2850
bitset 65536 6553 1 8192 1920
...
```

## Automação e Gráficos Gerados

O script `benchmarks/gerar_graficos.py` sintetiza os arquivos brutos e gera as figuras para a monografia em LaTeX:

- `benchmarks/figures/tempo_vs_universo.pdf`: Curvas de tempo ($ns/op$) vs. $u$ para operações de busca e sucessor.
- `benchmarks/figures/sucessor_comparativo.pdf`: Desempenho comparativo entre vEB, `std::set` e bitmap na operação `successor`.
- `benchmarks/figures/memoria_vs_universo.pdf`: Curva de footprint de memória em função do tamanho do universo $u$.
- `benchmarks/tables/tabela_desempenho_veb.tex`: Tabela compilada pronta para inclusão direta no relatório monográfico.
