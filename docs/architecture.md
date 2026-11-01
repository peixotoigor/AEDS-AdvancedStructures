# Arquitetura — Árvore de Van Emde Boas (vEB Tree)

## Visão Geral e Fundamentação Teórica

A **Árvore de Van Emde Boas (*vEB Tree*)** é uma estrutura de dados hierárquica recursiva projetada para manter conjuntos dinâmicos de inteiros pertencentes a um universo finito e limitado $U = [0, u - 1]$. Sob a restrição de universo limitado, ela atinge a complexidade de pior caso de $O(\log \log u)$ para todas as operações dinâmicas de dicionário ordenado (`member`, `insert`, `delete`, `predecessor`, `successor`), superando o limite inferior de $\Omega(\log N)$ inerente a modelos baseados em comparação pura (como árvores rubro-negras e heaps binários).

O princípio fundamental da árvore de Van Emde Boas reside na técnica de **divisão e conquista no espaço de bits** (*bit-level divide and conquer*), reduzindo iterativamente o tamanho do universo através da raiz quadrada: $u \to \sqrt{u} \to \sqrt{\sqrt{u}} \to \dots \to 2$.

## Estrutura do Repositório e Modularidade

```text
src/
├── core/
│   ├── van_emde_boas.cpp         # Implementação procedural das rotinas da vEB tree
│   └── main.cpp                  # Demonstração executável e verificação básica
└── include/
    └── van_emde_boas.hpp         # Structs, constantes, contratos e assinaturas livres
tests/
├── van_emde_boas_tests.cpp       # Suíte unitária funcional e validação de invariantes
└── benchmark_van_emde_boas.cpp   # Executor de medições de tempo e consumo de memória
benchmarks/
├── data/                         # Arquivos textuais (.txt) das medições empíricas
├── figures/                      # Gráficos de dispersão e custo (.pdf e .png)
├── tables/                       # Tabelas formatadas em LaTeX
├── gerar_graficos.py             # Script Python para agregação estatística
└── analise_graficos.ipynb        # Exploração interativa em Jupyter Notebook
docs/                             # Requisitos, arquitetura, testes e benchmarks
report/                           # Monografia científica em LaTeX
```

## Decomposição do Universo e Aritmética de Índices

Para acomodar qualquer dimensão baseada em potências de 2, o universo $u$ é definido como $u = 2^b$, onde $b \ge 1$. Quando $b$ é par (ou no caso canônico estrito $u = 2^{2^k}$), $\sqrt{u} = 2^{b/2}$ é inteiro exato. Quando $b$ é ímpar, utilizam-se as raízes quadradas superior e inferior:

$$\sqrt[\uparrow]{u} = 2^{\lceil b/2 
ceil}, \qquad \sqrt[\downarrow]{u} = 2^{\lfloor b/2 
floor}$$

Observe que $(\sqrt[\uparrow]{u}) \cdot (\sqrt[\downarrow]{u}) = 2^{\lceil b/2 
ceil + \lfloor b/2 
floor} = 2^b = u$.

### Funções de Decomposição Posicional

Cada chave $x \in [0, u - 1]$ é unicamente mapeada em um par $(h, l)$, onde $h$ indica o sub-bloco (*cluster*) e $l$ indica a posição relativa dentro deste sub-bloco:

1. **Componente de Cluster (`high`):**
   $$\text{high}(x) = \left\lfloor \frac{x}{\sqrt[\downarrow]{u}} 
ight
floor = x \gg \lfloor b/2 
floor$$
2. **Componente de Deslocamento (`low`):**
   $$\text{low}(x) = x \pmod{\sqrt[\downarrow]{u}} = x \ \& \ (\sqrt[\downarrow]{u} - 1)$$
3. **Reconstrução de Chave (`index`):**
   $$\text{index}(h, l) = h \cdot \sqrt[\downarrow]{u} + l = (h \ll \lfloor b/2 
floor) \mid l$$

## Representação Estrutural e Registros em C++20

Adotando paradigma procedural estrito, a estrutura de dados é definida por uma `struct` pura com campos explícitos e ponteiros gerenciados manualmente:

```cpp
#include <cstdint>

inline constexpr int64_t VEB_NIL = -1;

struct VanEmdeBoas {
    uint64_t u;                     // Tamanho do universo suportado por este nó
    int64_t min;                    // Menor elemento armazenado (ou VEB_NIL)
    int64_t max;                    // Maior elemento armazenado (ou VEB_NIL)
    VanEmdeBoas* summary;           // Ponteiro para vEB de resumo (universo sqrt_up(u))
    VanEmdeBoas** cluster;          // Vetor com sqrt_up(u) ponteiros (universo sqrt_down(u))
};
```

### O Papel Crítico de `min` e `max`

A chave para atingir complexidade $O(\log \log u)$ é o tratamento assimétrico dos extremos:

1. **Invariante do `min`:** O elemento armazenado em $min$ **NÃO** é inserido em nenhum dos clusters subordinados. Consequências diretas:
   - A consulta ao mínimo é $O(1)$.
   - A inserção de um elemento em uma árvore vazia requer apenas atribuir $min = max = x$, sem nenhuma chamada recursiva ($O(1)$).
   - Ao inserir em uma árvore não vazia onde o cluster de destino está vazio, insere-se o índice do cluster no $summary$ e insere-se a chave no cluster como seu único elemento ($min$), o que custa $O(1)$ e evita uma segunda chamada recursiva não trivial.
2. **Invariante do `max`:** O elemento armazenado em $max$ permite consultar o valor máximo em $O(1)$ e serve como limitador superior para verificar rapidamente se o sucessor de uma chave reside dentro do mesmo cluster ou em clusters posteriores.

## Operações e Análise de Complexidade Assintótica

### Recorrência Fundamental

Em cada operação dinâmica da vEB tree, a redução de tamanho segue $u \to \sqrt{u}$, gerando a relação de recorrência:

$$T(u) \le T(\sqrt{u}) + O(1)$$

Fazendo a substituição $u = 2^m$, onde $m = \lg u$:

$$T(2^m) \le T(2^{m/2}) + O(1)$$

Definindo $S(m) = T(2^m)$:

$$S(m) \le S(m/2) + O(1)$$

Pelo Teorema Mestre (Caso 2), $S(m) = O(\lg m)$. Retornando à variável original $m = \lg u$:

$$T(u) = O(\log \log u)$$

### Quadro Comparativo de Complexidades

| Operação | vEB Tree (Pior Caso) | `std::set` (Pior Caso) | `std::vector<bool>` / `bitset` |
|---|:---:|:---:|:---:|
| **Criar / Inicializar** | $O(u)$ (eager) / $O(1)$ (lazy) | $O(1)$ | $O(u)$ |
| **Consultar Vazio** | $O(1)$ | $O(1)$ | $O(1)$ |
| **Mínimo / Máximo** | $O(1)$ | $O(\log N)$ | $O(u)$ ou $O(u/64)$ |
| **Pertinência (`member`)** | $O(\log \log u)$ | $O(\log N)$ | $O(1)$ |
| **Sucessor (`successor`)** | $O(\log \log u)$ | $O(\log N)$ | $O(u)$ ou $O(u/64)$ |
| **Predecessor (`predecessor`)** | $O(\log \log u)$ | $O(\log N)$ | $O(u)$ ou $O(u/64)$ |
| **Inserção (`insert`)** | $O(\log \log u)$ | $O(\log N)$ | $O(1)$ |
| **Remoção (`delete`)** | $O(\log \log u)$ | $O(\log N)$ | $O(1)$ |
| **Destruição Completa** | $O(u)$ | $O(N)$ | $O(1)$ |
| **Consumo de Memória** | $O(u)$ nós estáticos / $O(N \log \log u)$ lazy | $O(N)$ | $u/8$ bytes |

## Algoritmos Conceituais Detalhados

### 1. Pertinência (`veb_pertence`)

Verifica se $x$ pertence ao conjunto:
- Se $x == V \to min$ ou $x == V \to max$, retorna `true` imediatamente em $O(1)$.
- Se $V \to u == 2$, retorna `false`.
- Caso contrário, delega recursivamente para o cluster correspondente: `veb_pertence(V->cluster[high(x)], low(x))`.
- Como há no máximo uma chamada recursiva para um universo de tamanho $\sqrt{u}$, o tempo é $O(\log \log u)$.

### 2. Sucessor (`veb_sucessor`)

Determina o menor elemento estritamente maior que $x$:
- Caso base $u = 2$: se $x == 0$ e $V \to max == 1$, retorna $1$; senão retorna $\text{VEB\_NIL}$.
- Se $V \to min \neq \text{VEB\_NIL}$ e $x < V \to min$, retorna $V \to min$ em $O(1)$.
- Se o cluster local $\text{high}(x)$ contém algum elemento maior que $\text{low}(x)$ (ou seja, se $\text{low}(x) < V \to cluster[\text{high}(x)] \to max$), o sucessor reside no próprio cluster local: calcula-se recursivamente `veb_sucessor(V->cluster[high(x)], low(x))`.
- Caso contrário, o sucessor está em um cluster posterior. Consulta-se o *summary* para descobrir o próximo cluster não vazio: $\text{succ\_cluster} = \text{veb\_sucessor}(V \to summary, \text{high}(x))$. Se não houver, retorna $\text{VEB\_NIL}$. Se houver, o menor elemento daquele cluster é $V \to cluster[\text{succ\_cluster}] \to min$, obtido em $O(1)$.
- Em qualquer ramificação, no máximo **uma** chamada recursiva é disparada. Complexidade: $O(\log \log u)$.

### 3. Predecessor (`veb_predecessor`)

Operação simétrica ao sucessor, com uma distinção fundamental decorrente do invariante do $min$: como $min$ não é propagado para dentro dos clusters, se a busca no cluster e no *summary* falhar, mas $x > V \to min$, então o predecessor é exatamente $V \to min$.

### 4. Inserção (`veb_inserir`)

- Se a árvore estiver vazia ($V \to min == \text{VEB\_NIL}$): atribui $V \to min = V \to max = x$ e encerra ($O(1)$).
- Se $x < V \to min$, realiza uma troca (*swap*) entre $x$ e $V \to min$, garantindo que o novo mínimo permaneça no topo sem descer para os clusters.
- Se $u > 2$:
  - Se o cluster de destino $V \to cluster[\text{high}(x)]$ estiver vazio, registra $\text{high}(x)$ no *summary* chamando `veb_inserir(V->summary, high(x))`, e em seguida define $min = max = \text{low}(x)$ naquele cluster vazio em tempo $O(1)$.
  - Se o cluster de destino já contiver elementos, apenas chama recursivamente `veb_inserir(V->cluster[high(x)], low(x))`.
- Se $x > V \to max$, atualiza $V \to max = x$.
- Em qualquer cenário, apenas **uma** chamada recursiva não trivial ocorre. Complexidade: $O(\log \log u)$.

### 5. Remoção (`veb_remover`)

- Se $V \to min == V \to max == x$, o elemento era único: redefine $V \to min = V \to max = \text{VEB\_NIL}$ e encerra.
- Caso base $u == 2$: se $x == 0$, $V \to min = 1$; se $x == 1$, $V \to min = 0$. Atualiza $V \to max = V \to min$.
- Se $x == V \to min$:
  - Localiza o primeiro cluster não vazio através de $\text{first\_cluster} = V \to summary \to min$.
  - Define $new\_min = \text{index}(\text{first\_cluster}, V \to cluster[\text{first\_cluster}] \to min)$.
  - Atualiza $V \to min = new\_min$, e agora remove a chave $new\_min$ do cluster de origem.
- Executa a remoção recursiva no cluster: `veb_remover(V->cluster[high(x)], low(x))`.
- Se após a remoção o cluster esvaziou ($V \to cluster[\text{high}(x)] \to min == \text{VEB\_NIL}$), remove esse cluster do *summary*: `veb_remover(V->summary, high(x))`.
- Se $x == V \to max$:
  - Se o *summary* estiver vazio, $V \to max = V \to min$.
  - Senão, localiza o último cluster não vazio $\text{last\_cluster} = V \to summary \to max$ e define $V \to max = \text{index}(\text{last\_cluster}, V \to cluster[\text{last\_cluster}] \to max)$.
- Como o cluster só esvazia quando possuía exatamente um elemento (e portanto sua remoção interna ocorreu em tempo constante no caso base), a chamada ao *summary* nunca coexiste com uma chamada recursiva profunda no cluster. Complexidade: $O(\log \log u)$.

## Invariantes Estruturais Formais

Para garantir corretude contínua, a árvore deve satisfazer os seguintes predicados de integridade estrutural após qualquer operação:

1. **Invariante de Vacuidade:**
   $$V \to min == \text{VEB\_NIL} \iff V \to max == \text{VEB\_NIL}$$
2. **Invariante de Elemento Singular:**
   $$V \to min == V \to max \iff |V| = 1 \iff (V \to summary == \text{nullptr} \lor V \to summary \to min == \text{VEB\_NIL})$$
3. **Invariante do Mínimo:**
   $$\forall i \in [0, \sqrt[\uparrow]{u}-1], \quad V \to cluster[i] \text{ não contém } V \to min$$
4. **Invariante do Resumo (*Summary Consistency*):**
   $$\text{veb\_pertence}(V \to summary, i) \iff V \to cluster[i] \to min \neq \text{VEB\_NIL}$$
5. **Invariante de Delimitação de Universo:**
   $$\forall x \in V, \quad 0 \le x < V \to u$$

## Estratégias de Alocação de Memória: Eager vs Lazy

1. **Alocação Eager (Canônica de Livro-Texto):**
   - Todos os ponteiros de clusters e summary são alocados no momento da criação da árvore.
   - Consumo espacial: $S(u) = (\sqrt{u} + 1) S(\sqrt{u}) + O(\sqrt{u}) = O(u)$.
   - Vantagens: Ausência de chamadas a `malloc`/`new` durante operações de busca e mutação, garantindo tempo de execução previsível e sem fragmentação durante o benchmark.
   - Desvantagens: Inviável para $u > 2^{28}$ em memória principal típica de desenvolvimento.
2. **Alocação Sob Demanda (*Lazy Allocation*):**
   - Clusters e summaries são inicializados com ponteiros nulos (`nullptr`) e instanciados apenas no primeiro `insert`.
   - Consumo espacial: $O(N \log \log u)$, tornando universos de $u = 2^{32}$ ou $u = 2^{64}$ viáveis quando esparsos.
