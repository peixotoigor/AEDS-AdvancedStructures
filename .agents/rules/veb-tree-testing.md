---
trigger: glob
description: "Testes, invariantes estruturais e differential testing da Árvore de van Emde Boas"
globs: "src/core/**/*.cpp, src/include/**/*.hpp, tests/**/*.cpp"
---

# Testes da Árvore de van Emde Boas

- Testar caso base $u = 2$ isoladamente (inserção, remoção, min, max, predecessor e sucessor).
- Testar árvore vazia, com um elemento e completamente cheia.
- Testar limites estritos do universo ($x = 0$ e $x = u - 1$), bem como tentativas inválidas ($x \ge u$).
- Testar operações repetidas (inserção de elemento já presente, remoção de inexistente).
- Validar propriedades de `min` e `max`: integridade de $O(1)$ e ausência de `min` nos clusters.
- Validar consistência do `summary` com os clusters ativos após cada inserção e remoção.
- Realizar *differential testing* comparando o comportamento de cada operação contra um oráculo canônico (`std::set<uint64_t>`).
- Utilizar entradas determinísticas e sementes registradas em testes randômicos.
- Executar sanitizers (ASan/UBSan) para validação de segurança de memória na destruição recursiva.