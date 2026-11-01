#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP

#include <cstddef>

/**
 * @brief Estrutura de um nó da árvore em um Fibonacci Heap.
 *
 * Cada nó armazena sua chave inteira, seu grau (número de filhos diretos),
 * uma flag booleana de marcação para controle do corte em cascata,
 * ponteiro para o nó pai, ponteiro para um de seus filhos,
 * e ponteiros esquerda/direita formando uma lista circular duplamente encadeada
 * com seus nós irmãos.
 */
struct NoFibonacci {
    int chave;
    int grau;
    bool marcado;
    NoFibonacci* pai;
    NoFibonacci* filho;
    NoFibonacci* esquerda;
    NoFibonacci* direita;
};

/**
 * @brief Estrutura principal do Fibonacci Heap.
 *
 * Mantém um ponteiro para a raiz de menor chave (minimo) e o total
 * de elementos alocados (quantidade).
 */
struct HeapFibonacci {
    NoFibonacci* minimo;
    int quantidade;
};

/**
 * @brief Cria e inicializa um novo Fibonacci Heap vazio.
 * @return Instância de HeapFibonacci com minimo=nullptr e quantidade=0.
 */
HeapFibonacci criar_heap();

/**
 * @brief Aloca e inicializa um nó isolado com a chave fornecida.
 * @param chave Valor inteiro a ser armazenado.
 * @return Ponteiro para o nó alocado com grau 0, desmarcado e auto-referenciado.
 */
NoFibonacci* criar_no(int chave);

/**
 * @brief Verifica se o heap está vazio.
 * @param heap Ponteiro para o heap.
 * @return true se o heap for nulo ou quantidade == 0; false caso contrário.
 */
bool heap_vazio(const HeapFibonacci* heap);

/**
 * @brief Insere uma chave no heap.
 *
 * Aloca um novo nó e o intercala na lista circular de raízes em O(1) amortizado.
 * @param heap Ponteiro para o heap.
 * @param chave Valor a ser inserido.
 * @return Ponteiro para o nó criado ou nullptr em caso de erro.
 */
NoFibonacci* inserir(HeapFibonacci* heap, int chave);

/**
 * @brief Consulta a chave mínima do heap sem removê-la.
 * @param heap Ponteiro para o heap.
 * @param chave_minima Ponteiro onde a chave mínima será gravada (pode ser nullptr).
 * @return true se o elemento mínimo existir; false se o heap estiver vazio ou for nulo.
 */
bool consultar_minimo(const HeapFibonacci* heap, int* chave_minima);

/**
 * @brief Une dois heaps em um novo heap em O(1) amortizado.
 *
 * Concatena as listas circulares de raízes de h1 e h2. Para evitar aliasing
 * e duplo delete, os heaps originais são esvaziados (minimo=nullptr, quantidade=0).
 * @param h1 Primeiro heap.
 * @param h2 Segundo heap.
 * @return Novo HeapFibonacci contendo todos os nós de h1 e h2.
 */
HeapFibonacci unir_heaps(HeapFibonacci* h1, HeapFibonacci* h2);

/**
 * @brief Extrai o nó de menor chave do heap em O(log n) amortizado.
 *
 * Promove os filhos do mínimo para a lista de raízes, remove e desaloca o nó mínimo,
 * e consolida árvores de mesmo grau até que todas as raízes tenham graus distintos.
 * @param heap Ponteiro para o heap.
 * @param chave_extraida Ponteiro onde a chave extraída será gravada (pode ser nullptr).
 * @return true se a extração foi bem-sucedida; false se o heap estiver vazio ou for nulo.
 */
bool extrair_minimo(HeapFibonacci* heap, int* chave_extraida);

/**
 * @brief Diminui o valor da chave de um nó em O(1) amortizado.
 *
 * Se a nova chave for menor que a do pai, o nó é cortado da lista de filhos e
 * inserido nas raízes. O corte em cascata é propagado caso o pai já estivesse marcado.
 * @param heap Ponteiro para o heap.
 * @param no Nó cuja chave será reduzida.
 * @param nova_chave Novo valor (deve ser <= no->chave).
 * @return true se a operação teve sucesso; false se nova_chave > no->chave ou ponteiro inválido.
 */
bool diminuir_chave(HeapFibonacci* heap, NoFibonacci* no, int nova_chave);

/**
 * @brief Remove um nó arbitrário do heap em O(log n) amortizado.
 *
 * Desvincula o nó de seu pai (se houver), propaga o corte em cascata, promove o nó
 * a mínimo e executa a extração e desalocação do nó.
 * @param heap Ponteiro para o heap.
 * @param no Nó a ser removido.
 * @return true se o nó foi removido com sucesso; false caso contrário.
 */
bool remover_no(HeapFibonacci* heap, NoFibonacci* no);

/**
 * @brief Desaloca todos os nós pertencentes ao heap e redefine seu estado para vazio.
 * @param heap Ponteiro para o heap a ser destruído.
 */
void destruir_heap(HeapFibonacci* heap);

/**
 * @brief Valida formalmente os invariantes estruturais do Fibonacci Heap.
 *
 * Verifica:
 * - Correspondência entre quantidade e total de nós;
 * - Circularidade e reciprocidade das listas (esquerda/direita);
 * - Ausência de pai nas raízes;
 * - Propriedade de min-heap (chave do pai <= chaves dos filhos);
 * - Correspondência exata entre o campo grau e a contagem de filhos diretos;
 * - Ponteiro minimo apontando para uma raiz de menor chave.
 * @param heap Ponteiro para o heap a ser validado.
 * @return true se todos os invariantes forem satisfeitos; false se houver inconsistência.
 */
bool verificar_invariantes(const HeapFibonacci* heap);

/* Operações estruturais auxiliares (expostas para validação e testes específicos) */
void ligar_arvores(NoFibonacci* y, NoFibonacci* x);
void cortar(HeapFibonacci* heap, NoFibonacci* x, NoFibonacci* y);
void corte_em_cascata(HeapFibonacci* heap, NoFibonacci* y);
void consolidar(HeapFibonacci* heap);

#endif // FIBONACCI_HEAP_HPP
