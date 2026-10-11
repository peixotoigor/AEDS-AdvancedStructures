---
name: academic-latex-report
description: Redige e revisa relatório técnico-científico em LaTeX sobre o Fibonacci Heap, com referências cruzadas, dados rastreáveis, complexidade e limitações explícitas
argument-hint: "Indique a seção, figura, tabela ou algoritmo do relatório"
---

# Relatório acadêmico em LaTeX

## Quando usar

Use esta skill ao criar ou revisar o relatório, pseudocódigo, figuras, tabelas,
análise experimental, bibliografia ou cheat sheet do projeto.

## Procedimento

1. Leia `docs/latex-writing-style.md` e `docs/requirements.md`.
2. Confirme no código e nos resultados quais operações foram realmente
   implementadas e medidas.
3. Escreva com linguagem direta, termos técnicos precisos e sem frases de
   preenchimento ou adjetivos promocionais.
4. Distinga custo de uma operação isolada, pior caso, caso médio e custo
   amortizado.
5. Para cada afirmação experimental, preserve o vínculo com dados brutos,
   tamanho da entrada, repetições, unidade e dispersão quando disponíveis.
6. Dê referência verificável a definições, algoritmos e fatos históricos.
7. Atribua `\label` a toda figura, tabela e algoritmo e cite cada artefato no
   texto com `\ref`.
8. Use legendas informativas, paleta sóbria e contraste que funcione em escala
   de cinza.
9. Declare limitações: alocação dinâmica, cache, hardware, STL sem
   `decrease-key` e divergências entre modelo assintótico e tempo observado.
10. Revise a consistência entre implementação, pseudocódigo, complexidades,
    tabelas, figuras, conclusões e bibliografia.

## Proibições

Não invente medições, referências, conclusões, figuras ou resultados. Quando
uma evidência não existir, descreva a ausência e limite a conclusão.
