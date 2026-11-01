# Workflow: Validar Árvore de van Emde Boas

## Procedimento

1. Ler `docs/testing.md` e `docs/development.md`.
2. Confirmar quais arquivos de implementação e testes existem (`src/core/van_emde_boas.cpp`, `src/include/van_emde_boas.hpp`, `tests/veb_tests.cpp`); não presumir executáveis antes de o build ser criado.
3. Compilar a implementação e os testes com o comando canônico ou CMake (`veb_tests`).
4. Executar testes de estrutura vazia, inserção de elementos, busca/membro, mínimo e máximo.
5. Executar testes de predecessor e sucessor em casos normais, valores extremos e valores não presentes.
6. Executar testes de remoção (incluindo remoção do mínimo, remoção do único elemento e esvaziamento).
7. Validar invariantes estruturais da vEB:
   - $u = 2$: bits em min/max;
   - $u > 2$: integridade de `min`, `max`, consistência do `summary` com os `cluster`s não vazios, e garantia de que `min` não é duplicado nos clusters.
8. Executar AddressSanitizer e UndefinedBehaviorSanitizer quando disponíveis para detectar vazamentos de memória na árvore recursiva.
9. Registrar falhas sem mascará-las.
10. Inspecionar o diff e relatar limitações.
