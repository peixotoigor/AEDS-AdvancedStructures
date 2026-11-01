---
name: veb-validator
description: Valida Árvore de van Emde Boas com testes determinísticos, invariantes estruturais (summary/cluster/min/max), referência STL e sanitizers
kind: local
model: inherit
subagent: true
---

# Validador da Árvore de van Emde Boas

## Procedimento

1. Leia `docs/testing.md` e `docs/development.md`.
2. Compile usando os comandos canônicos do repositório (`veb_tests`).
3. Execute testes de vEB vazia, inserção, busca de pertinência (`membro`), consulta de mínimo e máximo, predecessor, sucessor, remoção e destruição completa conforme implementados.
4. Após cada mutação (inserção/remoção), valide rigorosamente os invariantes estruturais:
   - Caso base $u = 2$: comportamento correto dos bits em `min` e `max`.
   - Caso geral $u > 2$: `min` e `max` coerentes; `min` não inserido recursivamente nos clusters; `summary` contém o índice $i$ se e somente se `cluster[i]` não está vazio.
5. Compare operações ordenadas com referências como `std::set<int>` apenas para checagem de corretude funcional das consultas de predecessor, sucessor, mínimo e máximo.
6. Execute AddressSanitizer e UndefinedBehaviorSanitizer para validar desalocação recursiva de clusters e summary.
7. Pare diante de falhas e preserve a saída real do comando.

## Saída

Informe comandos, casos executados, resultado, falhas com arquivo e linha, cobertura ausente e limitações do ambiente. Não corrija o código durante uma validação solicitada como revisão.
