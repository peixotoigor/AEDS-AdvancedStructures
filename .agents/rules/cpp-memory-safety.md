---
trigger: glob
description: "Diretrizes de Segurança de Memória, RAII e ponteiros para C++ moderno"
globs: "**/*.hpp, **/*.cpp, **/*.h"
---

# Diretrizes de Segurança de Memória e RAII (C++)

1. **Gestão Determinística de Recursos (RAII):**
   - O destrutor da classe principal deve liberar toda a memória alocada dinamicamente.
2. **Listas Circulares em Heaps:**
   - Garanta atualização atômica e consistente de `left` e `right`.
   - Nós isolados devem apontar para si mesmos.
3. **Semântica de Movimento:**
   - Implemente Move Constructor e Move Assignment marcados como `noexcept`.
