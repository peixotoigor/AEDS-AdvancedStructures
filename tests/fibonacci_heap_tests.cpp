#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include "fibonacci_heap.hpp"

// Macro simples para asserções com mensagem clara
#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FALHA NO TESTE: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            std::exit(1); \
        } \
    } while (0)

void test_heap_vazio() {
    std::cout << "[TESTE] Heap vazio e casos limite... ";
    HeapFibonacci h = criar_heap();
    TEST_ASSERT(heap_vazio(&h), "Heap recem-criado deve estar vazio");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes de heap vazio devem ser validos");

    int min_val = -1;
    TEST_ASSERT(!consultar_minimo(&h, &min_val), "Consultar minimo em heap vazio deve retornar false");
    TEST_ASSERT(!extrair_minimo(&h, &min_val), "Extrair minimo em heap vazio deve retornar false");
    TEST_ASSERT(!diminuir_chave(&h, nullptr, 10), "Diminuir chave com ponteiros nulos deve retornar false");
    TEST_ASSERT(!remover_no(&h, nullptr), "Remover no com ponteiro nulo deve retornar false");

    destruir_heap(&h);
    TEST_ASSERT(h.minimo == nullptr && h.quantidade == 0, "Heap destruido deve manter minimo nulo e quantidade zero");
    std::cout << "OK\n";
}

void test_insercao_um_elemento() {
    std::cout << "[TESTE] Insercao e extracao de um unico elemento... ";
    HeapFibonacci h = criar_heap();
    NoFibonacci* no = inserir(&h, 42);
    TEST_ASSERT(no != nullptr, "No inserido nao deve ser nulo");
    TEST_ASSERT(!heap_vazio(&h), "Heap com 1 elemento nao deve estar vazio");
    TEST_ASSERT(h.quantidade == 1, "Quantidade deve ser 1");
    TEST_ASSERT(h.minimo == no, "Minimo deve apontar para o unico no");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes com 1 no devem ser validos");

    int min_val = 0;
    TEST_ASSERT(consultar_minimo(&h, &min_val) && min_val == 42, "Minimo consultado deve ser 42");

    int extraido = 0;
    TEST_ASSERT(extrair_minimo(&h, &extraido) && extraido == 42, "Elemento extraido deve ser 42");
    TEST_ASSERT(heap_vazio(&h), "Heap deve ficar vazio apos extrair o unico elemento");
    TEST_ASSERT(h.quantidade == 0, "Quantidade deve ser 0");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem ser validos apos extracao");

    destruir_heap(&h);
    std::cout << "OK\n";
}

void test_insercao_multiplos_e_exemplo_relatorio() {
    std::cout << "[TESTE] Insercoes multiplas e consolidacao (exemplo do relatorio)... ";
    HeapFibonacci h = criar_heap();
    // Chaves do exemplo em report/sections/fibonacci-tree.tex: 7, 3, 18, 38, 10, 12
    inserir(&h, 7);
    inserir(&h, 3);
    inserir(&h, 18);
    inserir(&h, 38);
    inserir(&h, 10);
    inserir(&h, 12);

    TEST_ASSERT(h.quantidade == 6, "Quantidade de nos deve ser 6");
    int min_val = 0;
    TEST_ASSERT(consultar_minimo(&h, &min_val) && min_val == 3, "Minimo deve ser 3");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem ser validos apos insercoes");

    // Extrai o mínimo 3: consolidação deve ligar árvores
    int extraido = 0;
    TEST_ASSERT(extrair_minimo(&h, &extraido) && extraido == 3, "Minimo extraido deve ser 3");
    TEST_ASSERT(h.quantidade == 5, "Quantidade de nos deve ser 5 apos extracao");
    TEST_ASSERT(consultar_minimo(&h, &min_val) && min_val == 7, "Novo minimo deve ser 7");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem ser validos apos consolidacao");

    // Extrai o restante e confirma ordem ordenada
    std::vector<int> esperados = {7, 10, 12, 18, 38};
    for (int esp : esperados) {
        TEST_ASSERT(extrair_minimo(&h, &extraido), "Extracao deve ser bem-sucedida");
        TEST_ASSERT(extraido == esp, "Valor extraido deve coincidir com a ordenacao");
        TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem ser mantidos");
    }
    TEST_ASSERT(heap_vazio(&h), "Heap deve estar vazio ao final");
    destruir_heap(&h);
    std::cout << "OK\n";
}

void test_chaves_duplicadas() {
    std::cout << "[TESTE] Elementos com chaves duplicadas... ";
    HeapFibonacci h = criar_heap();
    std::vector<int> chaves = {5, 2, 8, 2, 5, 1, 8, 2};
    for (int c : chaves) {
        inserir(&h, c);
        TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem ser mantidos apos cada insercao");
    }

    std::sort(chaves.begin(), chaves.end());
    for (int esp : chaves) {
        int extraido = 0;
        TEST_ASSERT(extrair_minimo(&h, &extraido), "Extracao deve funcionar com duplicatas");
        TEST_ASSERT(extraido == esp, "Extracao deve respeitar ordem nao decrescente");
        TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem ser mantidos durante extracao com duplicatas");
    }
    TEST_ASSERT(heap_vazio(&h), "Heap deve esvaziar completamente");
    destruir_heap(&h);
    std::cout << "OK\n";
}

void test_uniao_heaps() {
    std::cout << "[TESTE] Uniao de heaps (casos vazio e populado)... ";
    // Caso 1: dois vazios
    HeapFibonacci h1 = criar_heap();
    HeapFibonacci h2 = criar_heap();
    HeapFibonacci u1 = unir_heaps(&h1, &h2);
    TEST_ASSERT(heap_vazio(&u1), "Uniao de dois vazios deve ser vazio");
    destruir_heap(&u1);

    // Caso 2: um vazio e um populado
    HeapFibonacci h3 = criar_heap();
    HeapFibonacci h4 = criar_heap();
    inserir(&h4, 15);
    inserir(&h4, 25);
    HeapFibonacci u2 = unir_heaps(&h3, &h4);
    TEST_ASSERT(u2.quantidade == 2, "Quantidade da uniao deve ser 2");
    TEST_ASSERT(h4.quantidade == 0 && h4.minimo == nullptr, "Heap original deve ter sido esvaziado");
    TEST_ASSERT(verificar_invariantes(&u2), "Invariantes de u2 devem ser validos");
    destruir_heap(&u2);

    // Caso 3: dois heaps populados
    HeapFibonacci h5 = criar_heap();
    HeapFibonacci h6 = criar_heap();
    inserir(&h5, 20);
    inserir(&h5, 5);
    inserir(&h5, 30);

    inserir(&h6, 12);
    inserir(&h6, 3);
    inserir(&h6, 18);

    HeapFibonacci u3 = unir_heaps(&h5, &h6);
    TEST_ASSERT(u3.quantidade == 6, "Total da uniao deve ser 6");
    TEST_ASSERT(h5.quantidade == 0 && h6.quantidade == 0, "Heaps de entrada devem ser zerados");
    int min_val = 0;
    TEST_ASSERT(consultar_minimo(&u3, &min_val) && min_val == 3, "Minimo da uniao deve ser 3");
    TEST_ASSERT(verificar_invariantes(&u3), "Invariantes da uniao devem ser validos");

    destruir_heap(&u3);
    std::cout << "OK\n";
}

void test_diminuir_chave_e_cortes() {
    std::cout << "[TESTE] Diminuicao de chave, corte e corte em cascata... ";
    HeapFibonacci h = criar_heap();
    // Insere elementos para formar arvores maiores apos extracao
    inserir(&h, 10);
    inserir(&h, 20);
    inserir(&h, 30);
    NoFibonacci* n40 = inserir(&h, 40);
    NoFibonacci* n50 = inserir(&h, 50);

    // Força consolidação ao extrair o mínimo (10)
    int ext = 0;
    extrair_minimo(&h, &ext);
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes apos consolidacao devem ser validos");

    // Tentativa invalida: aumentar a chave
    TEST_ASSERT(!diminuir_chave(&h, n40, 999), "Diminuir chave para valor maior deve falhar");
    TEST_ASSERT(n40->chave == 40, "Chave nao deve ter sido modificada");

    // Diminuir chave sem violar min-heap (ou de uma raiz)
    TEST_ASSERT(diminuir_chave(&h, n50, 45), "Diminuir chave valida deve ter sucesso");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem permanecer validos apos diminuir chave");

    // Diminuir chave para um novo mínimo absoluto
    TEST_ASSERT(diminuir_chave(&h, n40, 2), "Diminuir chave para 2 deve ter sucesso");
    int min_val = 0;
    TEST_ASSERT(consultar_minimo(&h, &min_val) && min_val == 2, "Novo minimo deve ser 2");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes devem permanecer validos");

    // Teste especifico de corte em cascata
    // Cria arvore de grau >= 2 e corta dois filhos sucessivos do mesmo no
    HeapFibonacci h_cascata = criar_heap();
    std::vector<NoFibonacci*> nos;
    for (int i = 1; i <= 8; ++i) {
        nos.push_back(inserir(&h_cascata, i * 10));
    }
    // Extrai o primeiro (10) para forçar consolidação em uma árvore binomial de grau 3 (20 é raiz)
    extrair_minimo(&h_cascata, &ext);
    TEST_ASSERT(verificar_invariantes(&h_cascata), "Invariantes apos consolidacao de h_cascata");

    // Localiza um nó que possua filhos e corte sucessivos filhos
    for (NoFibonacci* no : nos) {
        if (no != nullptr && no->filho != nullptr && no->grau >= 2) {
            NoFibonacci* f1 = no->filho;
            NoFibonacci* f2 = f1->direita;
            // Corta primeiro filho diminuindo chave
            diminuir_chave(&h_cascata, f1, 1);
            TEST_ASSERT(verificar_invariantes(&h_cascata), "Invariantes apos 1o corte");
            TEST_ASSERT(no->marcado == true || no->pai == nullptr, "No pai deve ser marcado ou raiz");

            // Corta segundo filho diminuindo chave: deve acionar corte em cascata
            diminuir_chave(&h_cascata, f2, 0);
            TEST_ASSERT(verificar_invariantes(&h_cascata), "Invariantes apos 2o corte (cascata)");
            break;
        }
    }

    destruir_heap(&h);
    destruir_heap(&h_cascata);
    std::cout << "OK\n";
}

void test_remocao_no() {
    std::cout << "[TESTE] Remocao arbitraria de nos... ";
    HeapFibonacci h = criar_heap();
    NoFibonacci* n1 = inserir(&h, 100);
    NoFibonacci* n2 = inserir(&h, 200);
    NoFibonacci* n3 = inserir(&h, 300);
    NoFibonacci* n4 = inserir(&h, 400);

    TEST_ASSERT(h.quantidade == 4, "Quantidade inicial deve ser 4");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes iniciais");

    // Remove nó intermediário n2 (200)
    TEST_ASSERT(remover_no(&h, n2), "Remocao de n2 deve ter sucesso");
    TEST_ASSERT(h.quantidade == 3, "Quantidade deve ser 3");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes apos remocao de n2");

    // Remove nó mínimo n1 (100)
    TEST_ASSERT(remover_no(&h, n1), "Remocao do minimo n1 deve ter sucesso");
    TEST_ASSERT(h.quantidade == 2, "Quantidade deve ser 2");
    int min_val = 0;
    TEST_ASSERT(consultar_minimo(&h, &min_val) && min_val == 300, "Novo minimo deve ser 300");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes apos remocao de n1");

    // Remove os nós restantes
    TEST_ASSERT(remover_no(&h, n4), "Remocao de n4");
    TEST_ASSERT(remover_no(&h, n3), "Remocao de n3");
    TEST_ASSERT(heap_vazio(&h), "Heap deve estar vazio apos remover todos os nos");

    destruir_heap(&h);
    std::cout << "OK\n";
}

void test_comparacao_referencia_stl() {
    std::cout << "[TESTE] Comparacao de referencia com std::priority_queue (1000 elementos deterministas)... ";
    HeapFibonacci h = criar_heap();
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq_ref;

    // Gerador determinístico de números com semente fixa conforme docs/testing.md
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-10000, 10000);

    const int TOTAL_ELEMENTOS = 1000;
    for (int i = 0; i < TOTAL_ELEMENTOS; ++i) {
        int v = dist(rng);
        inserir(&h, v);
        pq_ref.push(v);
    }

    TEST_ASSERT(h.quantidade == TOTAL_ELEMENTOS, "Total de elementos deve ser 1000");
    TEST_ASSERT(verificar_invariantes(&h), "Invariantes apos 1000 insercoes");

    for (int i = 0; i < TOTAL_ELEMENTOS; ++i) {
        int min_h = 0;
        int min_pq = pq_ref.top();
        pq_ref.pop();

        TEST_ASSERT(extrair_minimo(&h, &min_h), "Extracao no Fibonacci Heap nao deve falhar");
        TEST_ASSERT(min_h == min_pq, "Elemento extraido do FibHeap deve ser identico ao da priority_queue");

        // Verifica invariantes periodicamente para não degradar tempo de teste
        if (i % 100 == 0) {
            TEST_ASSERT(verificar_invariantes(&h), "Invariantes preservados durante extracoes em massa");
        }
    }

    TEST_ASSERT(heap_vazio(&h), "Heap deve estar vazio ao final");
    TEST_ASSERT(pq_ref.empty(), "Referencia deve estar vazia ao final");
    destruir_heap(&h);
    std::cout << "OK\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << " Executando Suite de Testes: Fibonacci Heap\n";
    std::cout << "========================================\n";

    test_heap_vazio();
    test_insercao_um_elemento();
    test_insercao_multiplos_e_exemplo_relatorio();
    test_chaves_duplicadas();
    test_uniao_heaps();
    test_diminuir_chave_e_cortes();
    test_remocao_no();
    test_comparacao_referencia_stl();

    std::cout << "========================================\n";
    std::cout << " TODOS OS TESTES PASSARAM COM SUCESSO!\n";
    std::cout << "========================================\n";
    return 0;
}
