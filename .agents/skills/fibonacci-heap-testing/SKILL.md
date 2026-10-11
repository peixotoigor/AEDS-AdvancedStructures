---
name: fibonacci-heap-testing
description: Cria e executa testes determinísticos para Fibonacci Heap em C++, cobrindo casos-limite, operações mutáveis, invariantes, referência STL e sanitizers
argument-hint: "Indique a operação ou o conjunto de casos a testar"
---

# Testes do Fibonacci Heap

## Quando usar

Use esta skill ao adicionar uma operação, investigar uma falha de ponteiros ou
validar uma mudança antes de executar benchmarks.

## Procedimento

1. Leia `docs/testing.md`, `docs/architecture.md` e
   `docs/development.md`.
2. Localize o executável ou mecanismo de teste existente antes de criar novos
   comandos.
3. Cubra heap vazio, um elemento, entrada normal, valores repetidos, limites,
   operações repetidas e pré-condições inválidas quando aplicável.
4. Teste a sequência específica da operação:
   - inserção, mínimo e extração;
   - união;
   - diminuir chave sem corte;
   - corte e corte em cascata;
   - remoção;
   - destruição vazia e não vazia.
5. Verifique invariantes após cada mutação importante usando
   `fibonacci-heap-invariants`.
6. Compare inserção e extração com `std::priority_queue` apenas quando as
   operações forem equivalentes. Não trate a STL como referência para
   `decrease-key`, cortes ou remoção por ponteiro.
7. Use entradas determinísticas. Registre a semente se aleatoriedade for
   necessária.
8. Compile, execute os testes focados e depois a suíte completa.
9. Execute AddressSanitizer e UndefinedBehaviorSanitizer quando disponíveis.
10. Interrompa ou sinalize claramente qualquer falha. Nunca masque erros com
    capturas genéricas ou valores de sucesso artificiais.

## Resultado esperado

Informe os comandos executados, os casos cobertos, as falhas encontradas e as
limitações restantes. Benchmarks só podem ser executados depois da validação
funcional.
