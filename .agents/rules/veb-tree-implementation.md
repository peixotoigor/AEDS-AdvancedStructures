---
trigger: glob
description: "Estilo procedural, invariantes e gerenciamento de memória da Árvore de van Emde Boas"
globs: "src/core/**/*.cpp, src/include/**/*.hpp"
---

# Implementação da Árvore de van Emde Boas (vEB)

- Usar `struct` e funções livres em estilo C++ procedural claro.
- Evitar classes com herança, polimorfismo e camadas de abstração desnecessárias.
- Universo $u$ deve ser potência de 2 ($u = 2^b$), com $u \ge 2$.
- Caso base formal para $u = 2$: não alocar subestruturas `summary` nem `cluster`.
- Para $u > 2$: decompor em $\sqrt[\uparrow]{u} = 2^{\lceil b/2 \rceil}$ clusters, cada um com tamanho $\sqrt[\downarrow]{u} = 2^{\lfloor b/2 \rfloor}$, e uma subárvore `summary` de tamanho $\sqrt[\uparrow]{u}$.
- Funções auxiliares de bits de custo $O(1)$: `high(x)`, `low(x)` e `index(h, l)`.
- Preservar invariantes: `min` nunca é duplicado dentro dos clusters subordinados; `summary` reflete estritamente quais clusters não estão vazios.
- Alocação e desalocação: toda estrutura dinâmica criada deve ser desalocada recursivamente por função dedicada (`veb_destruir`), evitando memory leaks.
- Documentar formalmente os custos teóricos ($O(1)$ para min/max, $O(\log \log u)$ para busca, sucessor, predecessor, inserção e remoção).