---
name: create-feature
description: Implementa uma nova operação do Fibonacci Heap em C++ procedural, com invariantes, testes e documentação consistentes
---

# Implementar operação do Fibonacci Heap

Use esta skill para adicionar ou completar uma operação do Fibonacci Heap sem
introduzir camadas, classes ou dependências que não pertencem ao projeto.

## Procedimento

1. Leia `docs/architecture.md`, `docs/coding-style.md` e `docs/testing.md`.
2. Confirme o contrato da operação, suas pré-condições e sua complexidade
   amortizada.
3. Atualize as declarações em `src/include/fibonacci_heap.hpp`.
4. Implemente a operação em `src/core/fibonacci_heap.cpp`.
5. Preserve as listas circulares, a reciprocidade de `esquerda` e `direita`,
   os ponteiros de pai e filho, o grau, as marcas, o mínimo e a quantidade.
6. Adicione casos de entrada vazia, um elemento, valores repetidos, operações
   repetidas e limites relevantes.
7. Verifique os invariantes depois das mutações importantes.
8. Compile e execute os testes focados antes de executar benchmarks.
9. Atualize a documentação diretamente afetada pela mudança.
10. Relate falhas reais, limitações e comandos executados; não use valores
    padrão que aparentem sucesso.

## Restrições

- Arquivos `.cpp` ficam em `src/core/`.
- Arquivos `.hpp` ficam em `src/include/`.
- Use `struct`, funções livres e ponteiros explícitos conforme o estilo do
  projeto.
- Não crie `src/domain`, `src/application`, controllers, ORM, Zod ou testes
  Vitest.
- Toda alocação deve ter uma liberação claramente definida e única.
