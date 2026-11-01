# Testes — Árvore de Van Emde Boas (vEB Tree)

## Objetivo

A suíte de testes estabelece a verificação formal de corretude funcional, integridade de invariantes estruturais e ausência de vazamentos de memória da **Árvore de Van Emde Boas (*vEB Tree*)**. Medições de tempo e escalabilidade de hardware são conduzidas separadamente conforme documentado em [`benchmarks.md`](benchmarks.md).

## Organização dos Testes

```text
tests/
└── van_emde_boas_tests.cpp       # Casos unitários, invariantes e differential testing
src/
├── core/
│   ├── van_emde_boas.cpp         # Implementação procedural do algoritmo
│   └── main.cpp                  # Exemplos interativos
└── include/
    └── van_emde_boas.hpp         # Contratos e declarações
```

## Matriz de Cobertura Funcional Obrigatória

Os testes devem exercitar sistematicamente as seguintes dimensões algorítmicas:

### 1. Caso Base ($u = 2$)
- Instanciação de árvore com $u = 2$ (sem subárvores summary ou clusters).
- Transições de estado completas:
  - Vazio $\to$ inserção de $0$ $\to$ inserção de $1$ (árvore cheia).
  - Consultas de pertinência (`veb_pertence`) para chaves $0$ e $1$.
  - Consultas de sucessor e predecessor nas bordas ($0 \to 1$, $1 \to \text{VEB\_NIL}$).
  - Remoções sucessivas até esvaziamento completo.

### 2. Decomposição Recursiva ($u \in \{4, 16, 64, 256, 65536\}$)
- Árvores com universos de potências quadradas perfeitas ($u = 2^{2^k}$) e universos de potências ímpares ($u = 2^{2k+1}$, onde $\sqrt[\uparrow]{u} \neq \sqrt[\downarrow]{u}$).
- Inserção do primeiro elemento ($min == max = x$).
- Inserção de chaves no mesmo cluster local.
- Inserção de chaves distribuídas em clusters alternados.
- Inserções em ordem crescente, decrescente e pseudoaleatória com semente fixa.

### 3. Dinâmica Crítica de Remoção
- Remoção do único elemento ($min == max$), retornando ao estado vazio.
- Remoção de elemento intermediário sem esvaziar o cluster de origem.
- Remoção que esvazia completamente um cluster, forçando a remoção recursiva correspondente no *summary*.
- Remoção do elemento $min$, validando a promoção do menor elemento do primeiro cluster ativo e sua exclusão interna.
- Remoção do elemento $max$, validando o recálculo do novo máximo a partir do último cluster ativo no *summary*.

### 4. Consultas Ordenadas (`sucessor` e `predecessor`)
- Consulta de sucessor para elemento menor que $min$ (retorna $min$).
- Consulta de sucessor para chave dentro do mesmo cluster quando $\text{low}(x) < cluster[\text{high}(x)] \to max$.
- Consulta de sucessor exigindo salto entre clusters via *summary*.
- Consulta de sucessor para elemento $\ge max$ (retorna $\text{VEB\_NIL}$).
- Consulta de predecessor simétrica, cobrindo o caso especial em que a chave é maior que $min$, mas nenhum cluster anterior é encontrado (retorna $min$).

### 5. Casos de Fronteira e Validação de Limites
- Tentativa de inserção e busca de chaves fora do universo ($x < 0$ ou $x \ge u$).
- Inserção duplicada de chave existente (idempotência sem alteração de estado).
- Remoção de chave inexistente (falha graciosa sem corrupção).
- Destruição de árvore vazia e de árvore parcialmente preenchida.

## Verificador Formal de Invariantes (`veb_verificar_invariantes`)

Após cada operação de mutação em ambiente de teste, uma rotina recursiva de validação estrutural inspeciona:

1. **Invariante de Vacuidade:** $V \to min == \text{VEB\_NIL} \iff V \to max == \text{VEB\_NIL}$.
2. **Invariante de Ordenação Extrema:** Se $V \to min \neq \text{VEB\_NIL}$, então $0 \le V \to min \le V \to max < V \to u$.
3. **Invariante de Não-Duplicação do Mínimo:** Para todo $i \in [0, \sqrt[\uparrow]{u} - 1]$, nenhum elemento igual a $V \to min$ existe dentro de $V \to cluster[i]$.
4. **Invariante de Presença do Máximo:** Se $V \to min \neq V \to max$, o elemento $V \to max$ deve estar presente no cluster $V \to cluster[\text{high}(V \to max)]$.
5. **Consistência do Resumo:** $\text{veb\_pertence}(V \to summary, i) == \text{true} \iff V \to cluster[i] \to min \neq \text{VEB\_NIL}$.
6. **Integridade Recursiva:** Todo cluster não nulo e o summary satisfazem todos os invariantes recursivamente até $u = 2$.

## Testes Diferenciais (*Differential Testing*) com Oráculo STL

Para garantir corretude absoluta contra erros sutis de implementação algorítmica:

- Utiliza-se um container canônico de referência: `std::set<int64_t>` e/ou `std::bitset<65536>`.
- Um gerador pseudoaleatório parametrizado com semente conhecida (ex: `std::mt19937_64(42)`) submete a árvore vEB e o oráculo STL a um fluxo misto de $100.000$ operações:
  - $30\%$ Inserções aleatórias;
  - $20\%$ Remoções aleatórias;
  - $20\%$ Consultas de pertinência (`member` vs `std::set::count`);
  - $15\%$ Consultas de sucessor (`successor` vs `std::set::upper_bound`);
  - $15\%$ Consultas de predecessor (`predecessor` vs iterador reverso de `lower_bound`).
- Ao término de cada operação, compara-se o retorno da vEB tree com o do oráculo. Qualquer divergência aborta o teste com emissão do histórico de operações e semente geradora.

## Detecção de Vazamentos e Sanitizers

- Testes devem ser compilados com flags de sanitização `-fsanitize=address,undefined`.
- Validação obrigatória de que ciclos sucessivos de `veb_criar` $\to$ inserção em massa $\to$ `veb_destruir` resultam em **zero bytes vazados** (*leak-free execution*).
