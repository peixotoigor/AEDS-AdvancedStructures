---
name: fibonacci-heap-invariants
description: Verifica invariantes estruturais de um Fibonacci Heap em C++, incluindo listas circulares, pais, filhos, graus, marcas, mínimo e ownership de memória
argument-hint: "Indique a operação ou o arquivo a validar"
---

# Invariantes do Fibonacci Heap

## Quando usar

Use esta skill após implementar ou alterar inserção, união, extração do mínimo,
diminuir chave, corte, corte em cascata, remoção ou destruição.

## Procedimento

1. Leia `docs/architecture.md` e identifique os campos reais de `NoFibonacci`
   e `HeapFibonacci`.
2. Leia a implementação em `src/core/` e não presuma nomes ou campos que não
   existam.
3. Para cada lista visitada, verifique que ela é circular, que termina no
   primeiro nó e que não excede um limite de segurança durante o teste.
4. Verifique a reciprocidade de `esquerda` e `direita`.
5. Verifique que cada filho aponta para o pai correto e que cada nó pertence a
   uma única lista.
6. Conte os filhos diretos e compare o resultado com `grau`.
7. Verifique a propriedade de min-heap entre cada pai e seus filhos.
8. Verifique que `minimo` é nulo exatamente quando `quantidade` é zero e, caso
   contrário, aponta para uma raiz com menor chave.
9. Após remoções, confirme que o nó removido não é alcançável e que foi
   desalocado uma única vez.
10. Execute os testes focados e sanitizers disponíveis. Registre qualquer
    violação com a operação que a produziu.

## Critérios de falha

Não considere a estrutura válida se houver ponteiro nulo inesperado, ciclo que
não retorna ao início, contagem inconsistente, nó removido alcançável ou
reciprocidade quebrada.
