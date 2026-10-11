# Benchmarks

## Objetivo

Os benchmarks medem o comportamento de desempenho do Fibonacci Heap para
entradas progressivamente maiores. Eles não verificam corretude e não
substituem os testes de [`testing.md`](testing.md).

## Organização

```text
benchmarks/
└── README.md                  # orientação opcional para execução

results/
├── raw/                       # CSV gerados diretamente pelas medições
├── tables/                    # tabelas usadas no relatório
└── figures/                   # gráficos usados no relatório
```

## Regras de medição

Cada benchmark deve:

- medir uma operação claramente identificada;
- usar entradas progressivamente maiores;
- repetir cada medição;
- informar a unidade de tempo;
- registrar tamanho da entrada e número de repetições;
- salvar os dados brutos em CSV;
- registrar condições relevantes do ambiente;
- executar apenas operações já validadas pelos testes.

Tamanhos iniciais sugeridos:

```text
100, 1.000, 10.000, 100.000
```

Ajustar os tamanhos conforme o tempo de execução e a memória disponíveis.

## Operações

Medir separadamente, quando implementadas:

- inserção;
- consulta do mínimo;
- união;
- extração do mínimo;
- diminuição de chave;
- sequência mista de operações.

Comparar com `std::priority_queue` somente nas operações equivalentes. A STL
não oferece `decrease-key` diretamente.

## Formato dos dados

Salvar os dados brutos em `results/raw/`.

```csv
structure,operation,input_size,run,time_microseconds
fibonacci_heap,insert,1000,1,42.5
fibonacci_heap,insert,1000,2,40.8
priority_queue,insert,1000,1,31.2
priority_queue,insert,1000,2,30.7
```

## Interpretação

O relatório deve explicar:

- se o crescimento observado é compatível com a análise assintótica;
- quais operações apresentaram melhor desempenho;
- quando o Fibonacci Heap foi mais lento ou mais rápido;
- o impacto de alocação dinâmica, cache e hardware;
- a diferença entre complexidade amortizada e uma execução isolada;
- as limitações do experimento.

As complexidades amortizadas de referência são:

| Operação | Complexidade |
|---|---:|
| Inserção | `O(1)` |
| Consulta do mínimo | `O(1)` |
| União | `O(1)` |
| Diminuição de chave | `O(1)` |
| Extração do mínimo | `O(log n)` |
| Remoção | `O(log n)` |
