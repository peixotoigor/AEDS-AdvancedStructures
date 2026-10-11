# Workflow: Validar Fibonacci Heap

## Procedimento

1. Ler `docs/testing.md` e `docs/development.md`.
2. Confirmar quais arquivos de implementação e testes existem; não presumir
   executáveis antes de o build ser criado.
3. Compilar a implementação e os testes com o comando definido em
   `docs/development.md`.
4. Executar testes de heap vazio, inserção, mínimo e extração quando
   implementados.
5. Executar testes de união, diminuir chave, corte e remoção quando
   implementados.
6. Validar listas circulares, pais, filhos, graus, marcas e quantidade.
7. Executar AddressSanitizer e UndefinedBehaviorSanitizer quando disponíveis.
8. Registrar falhas sem mascará-las.
9. Inspecionar o diff e relatar limitações.
