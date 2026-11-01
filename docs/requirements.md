# Requisitos do Projeto — Árvore de Van Emde Boas (vEB Tree)

## Objetivo

Especificar formalmente, estruturar e analisar a estrutura de dados **Árvore de Van Emde Boas (*vEB Tree*)** como projeto acadêmico de Estruturas de Dados Avançadas. O foco reside na corretude estrita dos invariantes, clareza algorítmica, garantia da complexidade de pior caso $O(\log \log u)$ para operações de dicionário ordenado e experimentação empírica reproduzível com baselines canônicos.

## Entregáveis

1. **Especificação de Arquitetura e Contratos:** Formalização matemática da decomposição do universo, funções auxiliares de bits e invariantes estruturais em [`docs/architecture.md`](architecture.md).
2. **Definições Procedurais em C++20:** Declaração de `struct` pura e assinaturas de funções livres em `src/include/van_emde_boas.hpp` (sem orientação a objetos complexa).
3. **Suíte de Testes Unitários e Invariantes:** Casos de teste para $u = 2$, potências de 2, validação de limites, estresse com gerador pseudoaleatório e differential testing contra `std::set` e `std::bitset`.
4. **Motor de Benchmarking e Métricas:** Coleta estatística de tempo ($10\times$ repetições, média $\mu$ e desvio $\sigma$) e footprint de memória para variações de universo $u$ e densidade de preenchimento $\alpha = N/u$.
5. **Pós-processamento e Relatório:** Scripts Python para geração de gráficos comparativos e relatório monográfico completo em LaTeX.

## Escopo Funcional

A estrutura de dados opera sobre um universo fixo de chaves inteiras $U = [0, u - 1]$, onde $u$ é restrito a $u = 2^{2^k}$ (caso canônico estrito) ou potências de 2 arbitrárias $u = 2^b$ ($b \ge 1$), com divisão de expoente:

$$\sqrt[\uparrow]{u} = 2^{\lceil b/2 
ceil}, \quad \sqrt[\downarrow]{u} = 2^{\lfloor b/2 
floor}$$

As operações fundamentais e contratos obrigatórios compreendem:

- **Criação e Inicialização (`veb_criar`):** Alocação recursiva da árvore para um universo $u$, inicializando $min = max = \text{VEB\_NIL}$, subárvore de resumo (*summary*) de universo $\sqrt[\uparrow]{u}$ e vetor de $\sqrt[\uparrow]{u}$ subárvores (*clusters*) de universo $\sqrt[\downarrow]{u}$. Para o caso base $u = 2$, não há summary nem clusters.
- **Destruição Completa (`veb_destruir`):** Desalocação recursiva em pós-ordem de todos os clusters e do summary, prevenindo vazamentos de memória e restaurando ponteiros nulos.
- **Verificação de Vacuidade (`veb_vazio`):** Retorna `true` se e somente se $min == \text{VEB\_NIL}$. Execução em $O(1)$.
- **Consulta de Extremos (`veb_minimo` e `veb_maximo`):** Retorna o menor e o maior elemento armazenados na árvore em tempo estrito $O(1)$, inspecionando os campos diretos $min$ e $max$.
- **Verificação de Pertinência (`veb_pertence` / `member`):** Testa se $x \in [0, u - 1]$ pertence ao conjunto em tempo $O(\log \log u)$.
- **Sucessor (`veb_sucessor`):** Determina o menor elemento estritamente maior que $x$ no universo em tempo $O(\log \log u)$. Retorna $\text{VEB\_NIL}$ caso não exista.
- **Predecessor (`veb_predecessor`):** Determina o maior elemento estritamente menor que $x$ no universo em tempo $O(\log \log u)$. Retorna $\text{VEB\_NIL}$ caso não exista.
- **Inserção (`veb_inserir`):** Adiciona o elemento $x \in [0, u - 1]$ ao conjunto em tempo $O(\log \log u)$, mantendo a regra de que o valor mínimo não é propagado recursivamente para os clusters.
- **Remoção (`veb_remover`):** Remove o elemento $x$ do conjunto em tempo $O(\log \log u)$, atualizando $min$, $max$ e a presença correspondente no *summary* caso o cluster de origem esvazie.

## Funções Auxiliares de Decomposição

A manipulação posicional deve implementar as funções elementares:

1. $\text{high}(x) = \lfloor x / \sqrt[\downarrow]{u} 
floor$: determina o índice do cluster ao qual $x$ pertence.
2. $\text{low}(x) = x \pmod{\sqrt[\downarrow]{u}}$: determina o offset de $x$ no cluster interior.
3. $\text{index}(h, l) = h \cdot \sqrt[\downarrow]{u} + l$: reconstrói a chave original a partir do cluster $h$ e offset $l$.

Para universos que são potências exatas de 2 ($u = 2^b$), essas operações devem permitir otimização por deslocamento e máscara de bits (*bitwise operations*):

$$\text{high}(x) = x \gg \lfloor b/2 
floor, \quad \text{low}(x) = x \ \& \ (\sqrt[\downarrow]{u} - 1), \quad \text{index}(h, l) = (h \ll \lfloor b/2 
floor) \mid l$$

## Casos de Borda e Tratamento de Erros

- **Limites de Universo:** Operações invocadas com chaves fora do intervalo $[0, u - 1]$ devem ser rejeitadas com código de erro ou sentinela explícito.
- **Árvore Vazia:** Consultas de `minimo`, `maximo`, `sucessor` e `predecessor` em árvore vazia devem retornar formalmente o sentinela $\text{VEB\_NIL} = -1$.
- **Árvore com Único Elemento:** Quando $min == max$, o elemento existe unicamente no campo $min$ e todos os clusters permanecem vazios.
- **Idempotência:** A inserção de elemento já existente não deve alterar a estrutura nem duplicar nós. A remoção de elemento inexistente deve retornar código indicativo de falha sem corromper invariantes.

## Baselines Comparativos de Desempenho

A árvore de Van Emde Boas deve ser avaliada contra estruturas de dados concorrentes:

1. **`std::set<uint32_t>` (Árvore Rubro-Negra balanceada):**
   - Operações em $O(\log N)$, onde $N$ é o número de elementos armazenados, independente do tamanho do universo $u$.
   - Custo de memória dinâmico proporcional a $N$ ($32$ a $48$ bytes por nó em arquiteturas de 64 bits).
2. **`std::vector<bool>` / `std::bitset` (Bitmap Denso):**
   - Operações de `member`, `insert` e `delete` em $O(1)$ temporal via manipulação direta de bits.
   - Operações ordenadas (`successor`, `predecessor`, `min`, `max`) com custo $O(u)$ no pior caso (ou $O(u/64)$ com instruções de contagem de bits como `_tzcnt_u64` / `__builtin_ctzll`).
   - Custo de memória rígido de $u/8$ bytes.
3. **Trade-offs Chave:** Destacar que a vEB tree atinge $O(\log \log u)$ estrito em tempo para todas as operações dinâmicas ordenadas, sacrificando espaço em comparação ao `std::set` quando $N \ll u$ (caso não se adote alocação sob demanda).

## Critérios de Qualidade Acadêmica

- **Determinismo e Reprodutibilidade:** Toda execução de testes e benchmarks deve permitir parametrização estrita de sementes e tamanhos de entrada.
- **Isolamento Modular:** A lógica procedural de gerenciamento da árvore deve ser completamente isolada dos scripts de benchmark e utilitários de E/S.
- **Integridade de Documentação:** Nenhuma afirmação de complexidade teórica deve divergir da implementação algorítmica real.
