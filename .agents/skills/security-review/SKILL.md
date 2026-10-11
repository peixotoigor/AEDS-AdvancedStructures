---
name: security-review
description: "Auditoria de segurança de memória em C++ para Fibonacci Heap, incluindo use-after-free, double-free, vazamentos e corrupção de ponteiros"
---

# Auditoria de segurança de memória

## Quando utilizar

Use ao alterar operações que removem nós, consolidam árvores, cortam listas,
destroem o heap ou manipulam ownership manual.

## Procedimento

1. Leia `AGENTS.md`, `docs/architecture.md` e
   `.agents/skills/security-review/checklist.md`.
2. Inspecione o diff e os caminhos afetados.
3. Procure `use-after-free`, `double-free`, vazamentos, ponteiros pendentes,
   acesso nulo inesperado, corrupção de `esquerda`/`direita` e ciclos sem
   término.
4. Verifique que cada nó é liberado exatamente uma vez e que a destruição
   alcança raízes, filhos e listas circulares.
5. Execute testes e sanitizers disponíveis; preserve a saída real.
6. Relate severidade, arquivo, linha, evidência, impacto e correção sugerida.

Não introduza correções durante uma auditoria somente de revisão.
