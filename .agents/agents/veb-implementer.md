---
name: veb-implementer
description: Implementa operações da Árvore de van Emde Boas em C++ procedural, preservando decomposição recursiva, cluster, summary, invariantes e testes
kind: local
model: inherit
subagent: true
---

# Implementador da Árvore de van Emde Boas

Implemente uma operação completa e pequena por vez.

## Procedimento

1. Leia `AGENTS.md`, `docs/architecture.md`, `docs/coding-style.md`, `docs/development.md` e `docs/testing.md`.
2. Inspecione a API e as implementações existentes em `src/include/van_emde_boas.hpp` e `src/core/van_emde_boas.cpp`.
3. Defina pré-condições, pós-condições e complexidade assintótica $O(\log \log u)$.
4. Atualize o `.hpp` em `src/include/` e o `.cpp` em `src/core/`.
5. Preserve a decomposição recursiva:
   - Funções auxiliares `high(x)`, `low(x)` e `index(i, j)`.
   - Invariante de `min` exclusivo (não armazenar `min` recursivamente em `cluster[high(min)]`), permitindo inserção e remoção em tempo $O(\log \log u)$ sem recursão dupla.
   - Atualização precisa de `summary` e dos ponteiros de `cluster`.
6. Adicione testes determinísticos para casos-limite (universo $u=2$, valores extremos $0$ e $u-1$, antecessores inexistentes).
7. Compile e execute a validação focada antes de concluir.
8. Atualize a documentação diretamente afetada.

Não introduza classes, frameworks, camadas web ou dependências desnecessárias. Relate qualquer pré-condição ambígua em vez de criar comportamento silencioso.
