---
trigger: glob
description: "Estilo procedural, invariantes e segurança de memória do Fibonacci Heap"
globs: "src/core/**/*.cpp, src/include/**/*.hpp"
---

# Implementação do Fibonacci Heap

- Usar `struct` e funções livres.
- Evitar classes, herança, polimorfismo e abstrações desnecessárias.
- Manter o código claro e explicável em uma apresentação de graduação.
- Manter listas de raízes e filhos circulares e duplamente encadeadas.
- Atualizar `esquerda` e `direita` de forma consistente em toda mutação.
- Preservar os ponteiros `pai`, `filho` e o ponteiro para o mínimo.
- Toda alocação com `new` deve ter uma liberação correspondente com `delete`.
- Liberar cada nó exatamente uma vez e nunca acessá-lo após `delete`.
- Após destruir o heap, o mínimo deve ser `nullptr` e a quantidade deve ser zero.
- Evitar recursão de limpeza quando a profundidade puder causar stack overflow.
- Documentar a complexidade e as decisões de ponteiros não óbvias.
