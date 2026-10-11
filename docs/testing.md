# Testes

## Objetivo

Os testes verificam a corretude do Fibonacci Heap. As medições de desempenho
devem ser feitas separadamente conforme [benchmarks.md](benchmarks.md).

## Organização

```text
tests/
└── fibonacci_heap_tests.cpp
src/
├── core/
│   ├── fibonacci_heap.cpp
│   └── main.cpp              # demonstrações pequenas
└── include/
    └── fibonacci_heap.hpp
```

Os testes não devem ser usados para medir desempenho.

## Operações e cobertura mínima

Testar, quando implementadas:

- criação de heap vazio;
- inserção do primeiro nó;
- inserção de várias chaves;
- consulta do mínimo;
- extração do mínimo;
- união de dois heaps;
- diminuição de chave sem corte;
- diminuição de chave com corte;
- corte em cascata;
- remoção de nó;
- destruição de heap vazio e não vazio.

Cobrir, quando aplicável:

- entrada vazia;
- um elemento;
- entrada normal;
- valores repetidos;
- valor inexistente;
- limites;
- operações repetidas;
- erros e pré-condições;
- comparação com uma referência confiável.

## Invariantes específicos

Depois de cada operação importante, verificar:

- `minimo` é nulo somente quando o heap está vazio;
- `quantidade` coincide com o número de nós;
- as listas circulares podem ser percorridas sem loop infinito;
- `esquerda` e `direita` são referências recíprocas;
- cada filho aponta para seu pai;
- graus e marcas estão consistentes;
- a chave do mínimo é menor ou igual às demais raízes.

Para testes de remoção, confirmar que nenhum nó removido continua alcançável.

## Testes de referência

Usar `std::priority_queue` configurada como min-heap para comparar inserção e
extração quando a operação for equivalente. A comparação não cobre diretamente
`decrease-key`, corte ou corte em cascata.

## Reprodutibilidade e validação

Testes devem ser determinísticos. Se houver aleatoriedade, registrar a semente
e os parâmetros usados.

1. Formatar e compilar.
2. Executar testes focados.
3. Executar a suíte completa.
4. Usar sanitizers ou ferramentas equivalentes quando houver risco de erro de
   memória ou comportamento indefinido.
5. Executar benchmarks somente depois dos testes funcionais.

Uma falha deve interromper ou sinalizar claramente a validação. Não mascarar
erros com capturas genéricas, valores padrão que pareçam sucesso ou exclusão
silenciosa de casos.
