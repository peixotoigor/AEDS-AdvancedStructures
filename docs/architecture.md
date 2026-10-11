# Arquitetura

## Estrutura escolhida

O projeto implementa um **Fibonacci Heap**, uma fila de prioridade formada por
uma coleção de árvores min-heap conectadas por listas circulares duplamente
encadeadas.

## Princípios

- Organizar o código por responsabilidade.
- Manter a implementação separada dos testes e dos experimentos.
- Preferir interfaces pequenas e funções fáceis de explicar.
- Reutilizar componentes existentes antes de criar novos.
- Evitar abstrações prematuras.

## Diretórios

```text
src/
├── core/
│   ├── fibonacci_heap.cpp
│   └── main.cpp
└── include/
    └── fibonacci_heap.hpp
docs/         Documentação, relatório e cheat sheet
results/      Dados, tabelas e gráficos
```

- `src/include/fibonacci_heap.hpp` contém os registros, declarações e contratos
  públicos.
- `src/core/fibonacci_heap.cpp` contém a implementação das operações.
- `src/core/main.cpp` contém testes locais, asserções e exemplos de uso.

Todo arquivo `.cpp` deve ficar em `src/core/`. Todo arquivo `.hpp` deve ficar
em `src/include/`.

## Representação

O heap deve manter:

- ponteiro para o nó mínimo;
- quantidade de nós;
- lista circular de raízes;
- em cada nó, chave, grau, marca, pai, filho, esquerda e direita.

As operações que alteram a estrutura devem receber ou atualizar o estado do
heap explicitamente. Uma chamada típica é:

```cpp
HeapFibonacci heap = criar_heap();

inserir(&heap, 10);
inserir(&heap, 3);
NoFibonacci* menor = extrair_minimo(&heap);

destruir_heap(&heap);
```

Os nomes das funções podem ser ajustados à implementação final.

## Operações e complexidades amortizadas

| Operação | Complexidade amortizada |
|---|---:|
| Criar heap | `O(1)` |
| Inserir | `O(1)` |
| Consultar mínimo | `O(1)` |
| Unir heaps | `O(1)` |
| Extrair mínimo | `O(log n)` |
| Diminuir chave | `O(1)` |
| Remover nó | `O(log n)` |

As complexidades devem ser explicadas no relatório e não tratadas como
complexidade de cada execução isolada.

## Invariantes

- A lista de raízes é circular e duplamente encadeada.
- Cada nó pertence a exatamente uma lista.
- O pai de cada filho aponta para a árvore correta.
- A chave de um pai é menor ou igual às chaves de seus filhos.
- `grau` corresponde ao número de filhos diretos.
- Um nó marcado possui a semântica definida pelo corte em cascata.
- `minimo` aponta para uma raiz de menor chave.
- `quantidade` corresponde ao total de nós alocados no heap.

## Operações críticas

### Extrair mínimo

Mover os filhos do mínimo para a lista de raízes, remover o mínimo e
consolidar árvores de mesmo grau até que a lista de raízes seja válida.

### Diminuir chave

Atualizar a chave, cortar o nó quando a propriedade de heap for violada e
aplicar corte em cascata ao ancestral marcado.

### União

Intercalar as listas circulares de raízes e atualizar o mínimo sem copiar nós.

## Contratos

Cada componente deve deixar claro:

- entradas aceitas;
- saída produzida;
- erros ou pré-condições;
- complexidade;
- regras de alteração de estado;
- limitações conhecidas.

Detalhes de implementação não devem ser expostos sem necessidade.
