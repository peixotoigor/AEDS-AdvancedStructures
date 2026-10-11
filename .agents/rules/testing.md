---
trigger: glob
description: "Diretrizes de testes algorítmicos e casos limites para estruturas de dados em C++"
globs: "tests/**"
---

# Diretrizes de Testes para Estruturas de Dados Avançadas

1. **Casos Limites:** Estrutura vazia, elemento único, chaves repetidas, limites do universo.
2. **Equivalência com a STL:** Validação cruzada contra `std::priority_queue` e `std::set`.
3. **Verificação de Memória:** Execução obrigatória com AddressSanitizer.
