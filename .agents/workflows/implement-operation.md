# Workflow: Implementar Operação

## Procedimento

1. Ler `AGENTS.md`, `docs/architecture.md` e `docs/coding-style.md`.
2. Definir o contrato da operação e os invariantes afetados (universo $u$, $min$, $max$, $summary$, $cluster$).
3. Adicionar demonstrações pequenas em `src/core/main.cpp` somente quando
   necessário; os testes funcionais devem ficar em `tests/veb_tests.cpp`.
4. Implementar a operação em `src/core/van_emde_boas.cpp`.
5. Atualizar declarações em `src/include/van_emde_boas.hpp`.
6. Compilar com warnings habilitados.
7. Executar os testes relevantes.
8. Atualizar a complexidade ($O(\log \log u)$) e a documentação quando necessário.
9. Inspecionar o diff antes de concluir.

## Critérios de conclusão

- A operação funciona nos casos normais e limites ($u=2$, valores extremos, ausência de elementos).
- Os ponteiros de cluster, summary e invariantes permanecem válidos.
- A memória alocada é liberada corretamente.
- Os testes foram executados e seus resultados são reais.
