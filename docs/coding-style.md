# Estilo de Código — Árvore de Van Emde Boas (vEB Tree)

## Paradigma

A Árvore de Van Emde Boas deve ser implementada com programação procedural estruturada em C++20, com foco didático e clareza conceitual compatível com o nível de uma disciplina avançada de graduação.

- Usar `struct` pura para representar a árvore de Van Emde Boas.
- Usar funções livres puras para todas as operações.
- Evitar classes com herança, polimorfismo ou encapsulamento excessivo que obscureça o fluxo de controle recursivo.
- Preferir controle de fluxo explícito, funções enxutas e de responsabilidade única.
- Escrever código que possa ser explicado linha a linha em uma apresentação acadêmica.

## Arquivos e Organização

```text
src/
├── core/
│   ├── van_emde_boas.cpp         # Implementação das rotinas procedurais
│   └── main.cpp                  # Demonstração simples e exemplos de uso
└── include/
    └── van_emde_boas.hpp         # Structs, constantes e assinaturas de funções livres
```

- `src/include/van_emde_boas.hpp` deve conter as structs, constantes (`VEB_NIL = -1`), macros inline/constexpr de bits e contratos públicos.
- `src/core/van_emde_boas.cpp` deve conter a implementação das operações algorítmicas.
- `src/core/main.cpp` deve conter somente demonstrações pequenas e exemplos de uso.
- `tests/` deve conter os testes funcionais, verificação de invariantes e testes com oráculos STL.

Todo arquivo `.cpp` deve residir em `src/core/`. Todo arquivo `.hpp` deve residir em `src/include/`.

## Registros Sugeridos

```cpp
#include <cstdint>

inline constexpr int64_t VEB_NIL = -1;

struct VanEmdeBoas {
    uint64_t u;                     // Tamanho do universo suportado (potência de 2)
    int64_t min;                    // Menor elemento presente (ou VEB_NIL)
    int64_t max;                    // Maior elemento presente (ou VEB_NIL)
    VanEmdeBoas* summary;           // Ponteiro para subárvore de resumo (universo sqrt_up(u))
    VanEmdeBoas** cluster;          // Array de sqrt_up(u) ponteiros para clusters (universo sqrt_down(u))
};
```

O significado dos campos deve permanecer formal:
- `u`: dimensão máxima do universo de chaves $[0, u - 1]$.
- `min`: menor chave ativa, mantida estritamente fora dos clusters subordinados.
- `max`: maior chave ativa.
- `summary`: ponteiro nulo se $u = 2$; senão aponta para subárvore vEB de universo $\sqrt[\uparrow]{u}$.
- `cluster`: ponteiro nulo se $u = 2$; senão aponta para vetor alocado dinamicamente com $\sqrt[\uparrow]{u}$ ponteiros para subárvores de universo $\sqrt[\downarrow]{u}$.

## Ponteiros e Gerenciamento de Memória

- Toda alocação recursiva de nós e vetores de ponteiros deve ocorrer na rotina `veb_criar(u)`.
- Toda desalocação deve ocorrer em pós-ordem rigorosa em `veb_destruir(VanEmdeBoas*& V)`:
  1. Desalocar recursivamente cada um dos $\sqrt[\uparrow]{u}$ clusters;
  2. Liberar o array de ponteiros `delete[] V->cluster`;
  3. Desalocar recursivamente o `summary`;
  4. Liberar o nó raiz `delete V`;
  5. Atribuir o ponteiro original a `nullptr`.
- Não acessar ponteiros após `delete`.

## Comentários Técnicos

Escrever comentários em português explicando decisões matemáticas não triviais, tais como:

- Otimizações bitwise para cálculo de `high`, `low` e `index` quando $u = 2^b$;
- O motivo da não inserção de $min$ dentro dos clusters subordinados;
- A garantia de que no máximo uma chamada recursiva ocorre durante `veb_inserir` e `veb_remover`;
- A lógica de transição quando um cluster esvazia e requer exclusão no `summary`;
- O tratamento de borda para o caso base $u = 2$.

Evitar comentários redundantes que apenas parafraseiem atribuições óbvias.
