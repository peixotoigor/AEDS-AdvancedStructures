# Estilo de Código

## Paradigma

O Fibonacci Heap deve ser implementado com programação procedural estruturada,
com foco didático e compatível com o nível de uma disciplina de graduação.

- Usar `struct` para representar nós e o heap.
- Usar funções livres para todas as operações.
- Evitar classes, herança, polimorfismo e padrões de projeto desnecessários.
- Preferir controle de fluxo explícito e funções curtas.
- Usar templates somente quando facilitarem a reutilização da implementação.
- Escrever código que possa ser explicado linha a linha em uma apresentação.

## Arquivos

```text
src/
├── core/
│   ├── fibonacci_heap.cpp
│   └── main.cpp
└── include/
    └── fibonacci_heap.hpp
```

- `src/include/fibonacci_heap.hpp` deve conter os registros, declarações e
  contratos públicos.
- `src/core/fibonacci_heap.cpp` deve conter a implementação das operações.
- `src/core/main.cpp` deve conter asserções, exemplos pequenos e testes locais.

Todo arquivo `.cpp` deve ficar em `src/core/`. Todo arquivo `.hpp` deve ficar
em `src/include/`.

## Registros sugeridos

```cpp
struct NoFibonacci {
    int chave;
    int grau;
    bool marcado;
    NoFibonacci* pai;
    NoFibonacci* filho;
    NoFibonacci* esquerda;
    NoFibonacci* direita;
};

struct HeapFibonacci {
    NoFibonacci* minimo;
    int quantidade;
};
```

Os nomes podem variar se o código existente adotar outra convenção, mas o
significado dos campos deve permanecer claro.

## Ponteiros e memória

- A lista de raízes deve ser circular e duplamente encadeada.
- Cada nó deve manter `esquerda` e `direita` consistentes.
- Um nó sem pai é raiz; um nó com pai pertence à lista de filhos desse pai.
- Toda alocação deve ocorrer em uma função de criação bem identificada.
- Toda remoção deve liberar o nó somente depois de religar a estrutura.
- A função de destruição deve liberar raízes, filhos e listas circulares
  exatamente uma vez.
- Após destruir o heap, `minimo` deve ser `nullptr` e `quantidade` deve ser
  zero.
- Não acessar ponteiros depois de `delete`.

## Comentários

Escrever comentários em português para explicar decisões não óbvias, como:

- inserção em uma lista circular;
- consolidação de árvores após `extrair_minimo`;
- corte e corte em cascata;
- atualização dos ponteiros do pai e do filho;
- motivo da marcação de um nó;
- manutenção do menor elemento.

Não comentar atribuições óbvias nem repetir o código em linguagem natural.
