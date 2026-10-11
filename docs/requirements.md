# Requisitos do Projeto

## Objetivo

Implementar e analisar um Fibonacci Heap como parte de um trabalho acadêmico,
com foco em corretude, clareza, complexidade amortizada e experimentação
reproduzível.

## Entregáveis

- Código-fonte funcional.
- Testes para os comportamentos principais e casos-limite.
- Análise de complexidade de tempo e espaço.
- Pseudocódigo e diagramas quando ajudarem a explicar a solução.
- Experimentos com entradas progressivamente maiores, quando aplicável.
- Dados e gráficos dos experimentos.
- Relatório e material de consulta rápida.

## Escopo funcional

O mínimo esperado é:

- criar e destruir o heap;
- inserir chaves;
- consultar o mínimo;
- extrair o mínimo;
- unir heaps;
- diminuir uma chave;
- remover um nó, se a representação permitir identificá-lo.

## Comparação

Comparar o comportamento e o custo experimental com uma fila de prioridade de
referência quando fizer sentido. Explicar que a STL não fornece diretamente
todas as operações específicas do Fibonacci Heap, especialmente `decrease-key`.

## Escopo

O escopo deve permanecer compatível com o objetivo acadêmico. Não adicionar
infraestrutura, dependências ou abstrações que não contribuam diretamente para
a implementação, a análise ou a apresentação.

## Critérios de qualidade

- O resultado deve ser verificável por testes.
- As afirmações de complexidade devem corresponder ao código.
- Os experimentos devem informar entradas, método de medição e condições
  relevantes.
- Falhas devem ser visíveis; não usar resultados falsos ou silenciosos.
