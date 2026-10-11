# Workflow: Corrigir Falha

## Objetivo

Corrigir uma falha de compilação, teste ou comportamento do Fibonacci Heap de
forma mínima, didática e verificável.

## Procedimento

1. Ler `AGENTS.md`.
2. Ler `docs/requirements.md`, `docs/coding-style.md` e `docs/testing.md`.
3. Reproduzir a falha com os comandos documentados.
4. Identificar a causa raiz.
5. Alterar somente os arquivos necessários.
6. Compilar novamente.
7. Executar os testes relevantes.
8. Executar sanitizer quando houver risco de erro de memória.
9. Validar os invariantes afetados.
10. Inspecionar o diff.
11. Registrar alteração, validação e limitações.

## Critérios de conclusão

- A falha foi reproduzida ou a impossibilidade foi registrada.
- A causa provável foi identificada.
- A correção é mínima e explicável.
- Os testes relevantes passaram.
- Nenhum resultado foi inventado.
- O diff foi revisado.
