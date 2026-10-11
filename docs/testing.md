# Testes

## Objetivo

Os testes verificam a corretude do Fibonacci Heap. As medições de desempenho
devem ser feitas separadamente conforme [benchmarks.md](benchmarks.md).

## Organização

```text
tests/
└── README.md                 # orientação opcional para os testes
src/
├── core/
│   ├── fibonacci_heap.cpp
│   └── main.cpp              # testes locais e exemplos
└── include/
    └── fibonacci_heap.hpp
```

Os testes não devem ser usados para medir desempenho.

## Operações do Fibonacci Heap

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

## Cobertura mínima

Testar, quando aplicável:

- entrada vazia;
- um elemento;
- entrada normal;
- valores repetidos;
- valor inexistente;
- limites;
- operações repetidas;
- erros e pré-condições;
- comparação com uma referência confiável.

## Operações do Fibonacci Heap

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

## Reprodutibilidade

Testes devem ser determinísticos. Se houver aleatoriedade, registrar a
semente e os parâmetros usados.

## Ordem de validação

1. Formatar e compilar.
2. Executar testes focados.
3. Executar a suíte completa.
4. Usar sanitizers ou ferramentas equivalentes quando houver risco de erro de
   memória ou comportamento indefinido.
5. Executar benchmarks somente depois dos testes funcionais.

## Falhas

Uma falha deve interromper ou sinalizar claramente a validação. Não mascarar
erros com capturas genéricas, valores padrão que pareçam sucesso ou exclusão
silenciosa de casos.

## Testes de desempenho

Benchmarks não substituem testes funcionais. Devem ser executados somente após
os testes funcionais passarem. Consulte [benchmarks.md](benchmarks.md).
