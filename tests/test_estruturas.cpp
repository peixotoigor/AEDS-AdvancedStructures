#include <iostream>
#include <cassert>
#include <vector>
#include "veb.hpp"
#include "heap_fibonacci.hpp"
#include "heap_dario.hpp"
#include "avl.hpp"
#include "rb.hpp"
#include "dijkstra.hpp"

using namespace std;

void testar_fibonacci() {
    cout << "[TESTE 1/6] Heap de Fibonacci... ";
    HeapFibonacci heap;
    assert(heap.esta_vazio());

    heap.insert(10, 1);
    heap.insert(5, 2);
    auto n3 = heap.insert(20, 3);
    heap.insert(2, 4);

    assert(!heap.esta_vazio());
    assert(heap.tamanho_heap() == 4);
    assert(heap.minimum()->chave == 2);
    assert(heap.minimum()->valor == 4);

    auto min_removido = heap.extract_min();
    assert(min_removido->chave == 2);
    delete min_removido;

    assert(heap.minimum()->chave == 5);

    heap.decrease_key(n3, 1); // no 3 passa a ter chave 1
    assert(heap.minimum()->chave == 1);
    assert(heap.minimum()->valor == 3);

    cout << "PASSOU (OK)" << endl;
}

void testar_dario() {
    cout << "[TESTE 2/6] Heap D-Ário (d=4)... ";
    HeapDArio heap(4);
    assert(heap.esta_vazio());

    heap.insert(15, 1);
    heap.insert(8, 2);
    heap.insert(30, 3);
    heap.insert(3, 4);

    assert(!heap.esta_vazio());
    assert(heap.minimum().chave == 3);
    assert(heap.minimum().valor == 4);

    auto min_removido = heap.extract_min();
    assert(min_removido.chave == 3);
    assert(heap.minimum().chave == 8);

    heap.decrease_key(3, 1); // valor 3 passa a ter chave 1
    assert(heap.minimum().chave == 1);
    assert(heap.minimum().valor == 3);

    cout << "PASSOU (OK)" << endl;
}

void testar_veb() {
    cout << "[TESTE 3/6] Árvore van Emde Boas (U=16)... ";
    vanEmdeBoas tree(16);

    veb_insert(&tree, 2);
    veb_insert(&tree, 3);
    veb_insert(&tree, 7);
    veb_insert(&tree, 14);

    assert(veb_minimum(&tree) == 2);
    assert(veb_maximum(&tree) == 14);
    assert(veb_member(&tree, 7) == true);
    assert(veb_member(&tree, 5) == false);

    assert(veb_successor(&tree, 3) == 7);
    assert(veb_successor(&tree, 7) == 14);
    assert(veb_successor(&tree, 14) == vEB_NIL);

    assert(veb_predecessor(&tree, 7) == 3);
    assert(veb_predecessor(&tree, 2) == vEB_NIL);

    veb_delete(&tree, 2);
    assert(veb_minimum(&tree) == 3);

    veb_delete(&tree, 14);
    assert(veb_maximum(&tree) == 7);

    cout << "PASSOU (OK)" << endl;
}

void testar_avl() {
    cout << "[TESTE 4/6] Árvore AVL... ";
    AVLTree avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(5);
    avl.insert(15);

    assert(avl.search(10));
    assert(!avl.search(99));
    assert(avl.minimum() == 5);
    assert(avl.maximum() == 20);

    assert(avl.successor(10) == 15);
    assert(avl.predecessor(10) == 5);

    avl.remove(5);
    assert(avl.minimum() == 10);
    assert(!avl.search(5));

    cout << "PASSOU (OK)" << endl;
}

void testar_rb() {
    cout << "[TESTE 5/6] Árvore Rubro-Negra... ";
    RedBlackTree rb;
    rb.insert(10);
    rb.insert(20);
    rb.insert(5);
    rb.insert(15);

    assert(rb.search(10));
    assert(!rb.search(99));
    assert(rb.minimum() == 5);
    assert(rb.maximum() == 20);

    assert(rb.successor(10) == 15);
    assert(rb.predecessor(10) == 5);

    rb.remove(5);
    assert(rb.minimum() == 10);
    assert(!rb.search(5));

    cout << "PASSOU (OK)" << endl;
}

void testar_dijkstra() {
    cout << "[TESTE 6/6] Dijkstra com Heap Fibonacci e D-Ário... ";
    Grafo g(5);
    // Grafo com 4 vértices (1-indexado):
    // 1 -> 2 (peso 1), 2 -> 3 (peso 2), 1 -> 3 (peso 5), 3 -> 4 (peso 1)
    g.adicionar_aresta(1, 2, 1);
    g.adicionar_aresta(2, 3, 2);
    g.adicionar_aresta(1, 3, 5);
    g.adicionar_aresta(3, 4, 1);

    auto res_fib = dijkstra_heap_fibonacci(g, 1);
    auto res_dario = dijkstra_heap_dario(g, 1, 4);

    assert(res_fib.distancias[1] == 0);
    assert(res_fib.distancias[2] == 1);
    assert(res_fib.distancias[3] == 3);
    assert(res_fib.distancias[4] == 4);

    assert(res_dario.distancias[1] == 0);
    assert(res_dario.distancias[2] == 1);
    assert(res_dario.distancias[3] == 3);
    assert(res_dario.distancias[4] == 4);

    cout << "PASSOU (OK)" << endl;
}

int main() {
    cout << "==================================================" << endl;
    cout << "   TESTES DE CORRETUDE DAS ESTRUTURAS DE DADOS    " << endl;
    cout << "==================================================" << endl;

    testar_fibonacci();
    testar_dario();
    testar_veb();
    testar_avl();
    testar_rb();
    testar_dijkstra();

    cout << "==================================================" << endl;
    cout << " [SUCESSO] 100% dos testes passaram com exito!    " << endl;
    cout << "==================================================" << endl;
    return 0;
}
