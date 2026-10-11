# Workflow: Implementar Operação

## Procedimento

1. Ler `AGENTS.md`, `docs/architecture.md` e `docs/coding-style.md`.
2. Definir o contrato da operação e os invariantes afetados.
3. Adicionar demonstrações pequenas em `src/core/main.cpp` somente quando
   necessário; os testes funcionais devem ficar em `tests/`.
4. Implementar a operação em `src/core/fibonacci_heap.cpp`.
5. Atualizar declarações em `src/include/fibonacci_heap.hpp`.
6. Compilar com warnings habilitados.
7. Executar os testes relevantes.
8. Atualizar a complexidade e a documentação quando necessário.
9. Inspecionar o diff antes de concluir.

## Critérios de conclusão

- A operação funciona nos casos normais e limites.
- Os ponteiros e invariantes permanecem válidos.
- A memória alocada é liberada corretamente.
- Os testes foram executados e seus resultados são reais.
