#include "fibonacci_heap.hpp"
#include <vector>
#include <unordered_set>
#include <functional>

HeapFibonacci criar_heap() {
    HeapFibonacci heap;
    heap.minimo = nullptr;
    heap.quantidade = 0;
    return heap;
}

NoFibonacci* criar_no(int chave) {
    NoFibonacci* no = new NoFibonacci;
    no->chave = chave;
    no->grau = 0;
    no->marcado = false;
    no->pai = nullptr;
    no->filho = nullptr;
    // Em uma lista circular de um único elemento, os ponteiros apontam para ele mesmo
    no->esquerda = no;
    no->direita = no;
    return no;
}

bool heap_vazio(const HeapFibonacci* heap) {
    return (heap == nullptr || heap->quantidade == 0 || heap->minimo == nullptr);
}

NoFibonacci* inserir(HeapFibonacci* heap, int chave) {
    if (heap == nullptr) {
        return nullptr;
    }

    NoFibonacci* novo_no = criar_no(chave);

    // Se o heap estiver vazio, o novo nó torna-se a única raiz e o mínimo
    if (heap->minimo == nullptr) {
        heap->minimo = novo_no;
    } else {
        // Intercala o novo nó na lista circular de raízes adjacente ao nó mínimo atual
        NoFibonacci* min_atual = heap->minimo;
        NoFibonacci* min_direita = min_atual->direita;

        novo_no->esquerda = min_atual;
        novo_no->direita = min_direita;
        min_atual->direita = novo_no;
        min_direita->esquerda = novo_no;

        // Atualiza o ponteiro de mínimo caso a nova chave seja menor
        if (novo_no->chave < heap->minimo->chave) {
            heap->minimo = novo_no;
        }
    }

    heap->quantidade++;
    return novo_no;
}

bool consultar_minimo(const HeapFibonacci* heap, int* chave_minima) {
    if (heap_vazio(heap)) {
        return false;
    }
    if (chave_minima != nullptr) {
        *chave_minima = heap->minimo->chave;
    }
    return true;
}

HeapFibonacci unir_heaps(HeapFibonacci* h1, HeapFibonacci* h2) {
    HeapFibonacci novo = criar_heap();

    // Caso base: ambos nulos ou vazios
    if ((h1 == nullptr || h1->minimo == nullptr) && (h2 == nullptr || h2->minimo == nullptr)) {
        if (h1 != nullptr) { h1->minimo = nullptr; h1->quantidade = 0; }
        if (h2 != nullptr) { h2->minimo = nullptr; h2->quantidade = 0; }
        return novo;
    }

    // Se h1 está vazio, herda h2
    if (h1 == nullptr || h1->minimo == nullptr) {
        novo = *h2;
        h2->minimo = nullptr;
        h2->quantidade = 0;
        return novo;
    }

    // Se h2 está vazio, herda h1
    if (h2 == nullptr || h2->minimo == nullptr) {
        novo = *h1;
        h1->minimo = nullptr;
        h1->quantidade = 0;
        return novo;
    }

    // Ambos contêm raízes: intercalar as duas listas circulares em O(1)
    NoFibonacci* min1 = h1->minimo;
    NoFibonacci* min2 = h2->minimo;
    NoFibonacci* min1_esq = min1->esquerda;
    NoFibonacci* min2_esq = min2->esquerda;

    // Conecta o fim de min1 ao início de min2 e vice-versa
    min1_esq->direita = min2;
    min2->esquerda = min1_esq;
    min2_esq->direita = min1;
    min1->esquerda = min2_esq;

    // O novo mínimo é a menor entre as duas raízes mínimas
    novo.minimo = (min2->chave < min1->chave) ? min2 : min1;
    novo.quantidade = h1->quantidade + h2->quantidade;

    // Limpa os heaps de origem para transferir ownership e evitar double-free
    h1->minimo = nullptr;
    h1->quantidade = 0;
    h2->minimo = nullptr;
    h2->quantidade = 0;

    return novo;
}

void ligar_arvores(NoFibonacci* y, NoFibonacci* x) {
    // Torna y filho de x em O(1)
    y->pai = x;

    if (x->filho == nullptr) {
        x->filho = y;
        y->esquerda = y;
        y->direita = y;
    } else {
        // Intercala y na lista circular de filhos de x
        NoFibonacci* c = x->filho;
        NoFibonacci* r = c->direita;

        y->esquerda = c;
        y->direita = r;
        c->direita = y;
        r->esquerda = y;
    }

    x->grau++;
    y->marcado = false;
}

void consolidar(HeapFibonacci* heap) {
    if (heap == nullptr || heap->minimo == nullptr) {
        return;
    }

    // Buffer de graus: log_phi(2^31 - 1) < 45. 64 posições é suficiente para qualquer N viável.
    const int MAX_GRAU = 64;
    NoFibonacci* A[MAX_GRAU];
    for (int i = 0; i < MAX_GRAU; ++i) {
        A[i] = nullptr;
    }

    // 1. Coleta todas as raízes presentes antes das ligações
    // Isso evita navegar por ponteiros alterados dinamicamente durante a consolidação
    std::vector<NoFibonacci*> raizes;
    NoFibonacci* atual = heap->minimo;
    do {
        raizes.push_back(atual);
        atual = atual->direita;
    } while (atual != heap->minimo);

    // 2. Itera sobre cada raiz e une árvores com graus idênticos
    for (NoFibonacci* w : raizes) {
        NoFibonacci* x = w;
        int d = x->grau;

        while (d < MAX_GRAU && A[d] != nullptr) {
            NoFibonacci* y = A[d];
            if (x->chave > y->chave) {
                // x deve ser a raiz de menor chave para manter a propriedade de min-heap
                NoFibonacci* temp = x;
                x = y;
                y = temp;
            }

            // y se torna filho de x
            ligar_arvores(y, x);

            A[d] = nullptr;
            d++;
        }

        if (d < MAX_GRAU) {
            A[d] = x;
        }
    }

    // 3. Reconstrói a lista circular de raízes a partir das árvores consolidadas em A
    heap->minimo = nullptr;
    for (int i = 0; i < MAX_GRAU; ++i) {
        if (A[i] != nullptr) {
            A[i]->pai = nullptr;
            if (heap->minimo == nullptr) {
                heap->minimo = A[i];
                A[i]->esquerda = A[i];
                A[i]->direita = A[i];
            } else {
                NoFibonacci* min_no = heap->minimo;
                NoFibonacci* min_dir = min_no->direita;

                A[i]->esquerda = min_no;
                A[i]->direita = min_dir;
                min_no->direita = A[i];
                min_dir->esquerda = A[i];

                if (A[i]->chave < heap->minimo->chave) {
                    heap->minimo = A[i];
                }
            }
        }
    }
}

bool extrair_minimo(HeapFibonacci* heap, int* chave_extraida) {
    if (heap == nullptr || heap->minimo == nullptr) {
        return false;
    }

    NoFibonacci* z = heap->minimo;
    if (chave_extraida != nullptr) {
        *chave_extraida = z->chave;
    }

    // 1. Promove todos os filhos de z para a lista de raízes
    if (z->filho != nullptr) {
        NoFibonacci* f_atual = z->filho;
        std::vector<NoFibonacci*> filhos;
        do {
            filhos.push_back(f_atual);
            f_atual = f_atual->direita;
        } while (f_atual != z->filho);

        for (NoFibonacci* f : filhos) {
            f->pai = nullptr;
            // Insere f na lista de raízes logo após z
            NoFibonacci* z_dir = z->direita;
            f->esquerda = z;
            f->direita = z_dir;
            z->direita = f;
            z_dir->esquerda = f;
        }
        z->filho = nullptr;
        z->grau = 0;
    }

    // 2. Remove z da lista de raízes
    if (z->direita == z) {
        // z era a única raiz restante no heap
        heap->minimo = nullptr;
    } else {
        z->esquerda->direita = z->direita;
        z->direita->esquerda = z->esquerda;
        heap->minimo = z->direita;
    }

    delete z;
    heap->quantidade--;

    // 3. Se ainda houver elementos, consolida as raízes de graus iguais
    if (heap->minimo != nullptr) {
        consolidar(heap);
    }

    return true;
}

void cortar(HeapFibonacci* heap, NoFibonacci* x, NoFibonacci* y) {
    // 1. Remove x da lista circular de filhos de y
    if (x->direita == x) {
        y->filho = nullptr;
    } else {
        if (y->filho == x) {
            y->filho = x->direita;
        }
        x->esquerda->direita = x->direita;
        x->direita->esquerda = x->esquerda;
    }
    y->grau--;

    // 2. Insere x na lista de raízes do heap
    NoFibonacci* min_no = heap->minimo;
    NoFibonacci* min_dir = min_no->direita;

    x->esquerda = min_no;
    x->direita = min_dir;
    min_no->direita = x;
    min_dir->esquerda = x;

    x->pai = nullptr;
    x->marcado = false;
}

void corte_em_cascata(HeapFibonacci* heap, NoFibonacci* y) {
    NoFibonacci* z = y->pai;
    if (z != nullptr) {
        if (!y->marcado) {
            // Primeiro filho perdido: apenas marca o nó
            y->marcado = true;
        } else {
            // Segundo filho perdido: corta y do pai z e propaga em cascata
            cortar(heap, y, z);
            corte_em_cascata(heap, z);
        }
    }
}

bool diminuir_chave(HeapFibonacci* heap, NoFibonacci* no, int nova_chave) {
    if (heap == nullptr || no == nullptr) {
        return false;
    }
    // Não permite aumentar a chave
    if (nova_chave > no->chave) {
        return false;
    }

    no->chave = nova_chave;
    NoFibonacci* y = no->pai;

    // Se violar a propriedade de min-heap com relação ao pai, realiza corte
    if (y != nullptr && no->chave < y->chave) {
        cortar(heap, no, y);
        corte_em_cascata(heap, y);
    }

    // Atualiza o ponteiro minimo se a nova chave for a menor do heap
    if (heap->minimo == nullptr || no->chave < heap->minimo->chave) {
        heap->minimo = no;
    }

    return true;
}

bool remover_no(HeapFibonacci* heap, NoFibonacci* no) {
    if (heap == nullptr || no == nullptr || heap->minimo == nullptr) {
        return false;
    }

    // Se o nó possui pai, desvincula-o e propaga corte em cascata
    if (no->pai != nullptr) {
        NoFibonacci* y = no->pai;
        cortar(heap, no, y);
        corte_em_cascata(heap, y);
    }

    // Define no como o mínimo temporário para forçar sua extração e desalocação
    heap->minimo = no;
    int dummy = 0;
    return extrair_minimo(heap, &dummy);
}

static void destruir_subarvore(NoFibonacci* no) {
    if (no == nullptr) {
        return;
    }

    if (no->filho != nullptr) {
        NoFibonacci* atual = no->filho;
        std::vector<NoFibonacci*> filhos;
        do {
            filhos.push_back(atual);
            atual = atual->direita;
        } while (atual != no->filho);

        for (NoFibonacci* f : filhos) {
            destruir_subarvore(f);
        }
    }

    delete no;
}

void destruir_heap(HeapFibonacci* heap) {
    if (heap == nullptr || heap->minimo == nullptr) {
        if (heap != nullptr) {
            heap->minimo = nullptr;
            heap->quantidade = 0;
        }
        return;
    }

    std::vector<NoFibonacci*> raizes;
    NoFibonacci* atual = heap->minimo;
    do {
        raizes.push_back(atual);
        atual = atual->direita;
    } while (atual != heap->minimo);

    for (NoFibonacci* r : raizes) {
        destruir_subarvore(r);
    }

    heap->minimo = nullptr;
    heap->quantidade = 0;
}

bool verificar_invariantes(const HeapFibonacci* heap) {
    if (heap == nullptr) {
        return false;
    }

    // Heap vazio deve ter minimo nulo e quantidade zero
    if (heap->quantidade == 0) {
        return heap->minimo == nullptr;
    }
    if (heap->minimo == nullptr) {
        return false;
    }

    // 1. Percorrer a lista de raízes e verificar circularidade e reciprocidade
    std::unordered_set<const NoFibonacci*> raizes_visitadas;
    const NoFibonacci* atual = heap->minimo;

    do {
        if (atual == nullptr) {
            return false;
        }
        // Reciprocidade esquerda / direita
        if (atual->direita == nullptr || atual->esquerda == nullptr) {
            return false;
        }
        if (atual->direita->esquerda != atual || atual->esquerda->direita != atual) {
            return false;
        }
        // Raiz não pode possuir pai
        if (atual->pai != nullptr) {
            return false;
        }
        // Chave de qualquer raiz deve ser maior ou igual à raiz mínima
        if (atual->chave < heap->minimo->chave) {
            return false;
        }
        // Detecção de repetição indevida na lista circular
        if (raizes_visitadas.count(atual)) {
            return false;
        }

        raizes_visitadas.insert(atual);
        atual = atual->direita;
    } while (atual != heap->minimo);

    // 2. Verificar cada árvore recursivamente
    int total_nos = 0;
    std::unordered_set<const NoFibonacci*> todos_nos;

    std::function<bool(const NoFibonacci*)> valida_subarvore = [&](const NoFibonacci* no) -> bool {
        total_nos++;
        if (todos_nos.count(no)) {
            return false; // Nó visitado mais de uma vez (ciclo ou referência compartilhada)
        }
        todos_nos.insert(no);

        if (no->filho == nullptr) {
            return no->grau == 0;
        }

        // Valida lista circular de filhos
        const NoFibonacci* f_atual = no->filho;
        int cont_filhos = 0;
        std::unordered_set<const NoFibonacci*> filhos_visitados;

        do {
            if (f_atual == nullptr || f_atual->direita == nullptr || f_atual->esquerda == nullptr) {
                return false;
            }
            if (f_atual->direita->esquerda != f_atual || f_atual->esquerda->direita != f_atual) {
                return false;
            }
            if (f_atual->pai != no) {
                return false;
            }
            // Propriedade min-heap: filho deve ser maior ou igual ao pai
            if (f_atual->chave < no->chave) {
                return false;
            }
            if (filhos_visitados.count(f_atual)) {
                return false;
            }
            filhos_visitados.insert(f_atual);
            cont_filhos++;

            if (!valida_subarvore(f_atual)) {
                return false;
            }

            f_atual = f_atual->direita;
        } while (f_atual != no->filho);

        return cont_filhos == no->grau;
    };

    for (const NoFibonacci* r : raizes_visitadas) {
        if (!valida_subarvore(r)) {
            return false;
        }
    }

    // 3. Contagem total deve corresponder exatamente ao campo quantidade
    if (total_nos != heap->quantidade) {
        return false;
    }

    return true;
}
