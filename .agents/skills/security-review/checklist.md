# Checklist de Segurança de Memória

- [ ] Ponteiros são validados antes de acesso quando a API permitir entrada nula.
- [ ] `esquerda` e `direita` permanecem recíprocos após cada mutação.
- [ ] Um nó removido é desconectado antes de ser liberado.
- [ ] Nenhum ponteiro é acessado depois de `delete`.
- [ ] Cada nó é liberado exatamente uma vez.
- [ ] A destruição percorre raízes, filhos e listas circulares sem duplicar nós.
- [ ] Não há `use-after-free`, `double-free` ou vazamento observável.
- [ ] A contagem de nós e o ponteiro `minimo` são atualizados após remoções.
- [ ] Loops de listas circulares possuem condição de término verificável.
- [ ] Testes executam com AddressSanitizer e UndefinedBehaviorSanitizer quando disponíveis.
