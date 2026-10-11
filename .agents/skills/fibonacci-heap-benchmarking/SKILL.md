---
name: fibonacci-heap-benchmarking
description: Planeja e registra benchmarks reproduzíveis de Fibonacci Heap em C++, com CSV bruto, repetições, comparação limitada com priority_queue e análise amortizada
argument-hint: "Indique a operação e os tamanhos de entrada"
---

# Benchmarks do Fibonacci Heap

## Quando usar

Use esta skill para medir desempenho de operações já aprovadas pelos testes
funcionais. Não use benchmarks para diagnosticar corretude.

## Procedimento

1. Leia `docs/benchmarks.md`, `docs/testing.md` e
   `docs/development.md`.
2. Confirme que os testes funcionais e os sanitizers relevantes passaram.
3. Escolha uma operação, tamanhos progressivos e número explícito de
   repetições. Use entradas determinísticas.
4. Meça a operação com uma unidade declarada e controle o custo de preparação
   da entrada.
5. Registre cada execução em `results/raw/` com estrutura, operação, tamanho,
   repetição e tempo.
6. Compare com `std::priority_queue` somente em operações equivalentes.
7. Preserve os dados brutos antes de gerar tabelas ou figuras.
8. Relacione o resultado observado com a complexidade amortizada sem afirmar
   que uma única execução prova a análise assintótica.
9. Registre compilador, flags, hardware, sistema, tamanhos e repetições.
10. Explique limitações como alocação dinâmica, cache, aquecimento e custo de
    medição.

## Restrições

Não invente valores, descarte medições sem justificar, nem altere resultados
para aproximá-los da teoria. Dados ausentes devem ser apresentados como
limitação.
