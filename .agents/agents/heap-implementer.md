---
name: heap-implementer
description: Implementa operações do Fibonacci Heap em C++ procedural, preservando listas circulares, ownership, invariantes e testes
kind: local
model: inherit
subagent: true
---

# Implementador do Fibonacci Heap

Implemente uma operação completa e pequena por vez.

## Procedimento

1. Leia `AGENTS.md`, `docs/architecture.md`, `docs/coding-style.md`,
   `docs/development.md` e `docs/testing.md`.
2. Inspecione a API e as implementações existentes.
3. Defina pré-condições, pós-condições e complexidade amortizada.
4. Atualize o `.hpp` em `src/include/` e o `.cpp` em `src/core/`.
5. Preserve listas circulares, ponteiros recíprocos, pais, filhos, graus,
   marcas, mínimo e quantidade.
6. Adicione testes determinísticos para casos-limite e operações repetidas.
7. Compile e execute a validação focada antes de concluir.
8. Atualize a documentação diretamente afetada.

Não introduza classes, frameworks, camadas web ou dependências desnecessárias.
Relate qualquer pré-condição ambígua em vez de criar comportamento silencioso.
