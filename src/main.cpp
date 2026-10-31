#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <random>
#include <chrono>
#include <iomanip>
#include <stdexcept>

#include "dijkstra.hpp"
#include "heap_dario.hpp"
#include "heap_fibonacci.hpp"

using namespace std;

bool validarResultadoQuatruplo(const ResultadoDijkstra& res_d2, 
                               const ResultadoDijkstra& res_d4, 
                               const ResultadoDijkstra& res_d8, 
                               const ResultadoDijkstra& res_fib, 
                               const string& nome_teste) {
    size_t n = res_d2.distancias.size();
    if (res_d4.distancias.size() != n || res_d8.distancias.size() != n || res_fib.distancias.size() != n) {
        cout << " [ERRO - " << nome_teste << "] Tamanhos de vetores de distancia divergentes!" << endl;
        return false;
    }

    for (size_t i = 0; i < n; i++) {
        int d2 = res_d2.distancias[i];
        int d4 = res_d4.distancias[i];
        int d8 = res_d8.distancias[i];
        int fib = res_fib.distancias[i];

        if (d2 != d4 || d2 != d8 || d2 != fib) {
            cout << " [ERRO - " << nome_teste << "] Divergencia no vertice " << i 
                 << ": d=2 (" << d2 << ") | d=4 (" << d4 << ") | d=8 (" << d8 << ") | Fib (" << fib << ")" << endl;
            return false;
        }
    }
    cout << " [OK] " << nome_teste << ": d=2, d=4, d=8 e Fibonacci produziram resultados identicos!" << endl;
    return true;
}


void testeGrafoPadrao() {
    cout << "\n--------------------------------------------------" << endl;
    cout << " [1] Teste Grafo Dirigido com Caminho Conhecido" << endl;
    cout << "--------------------------------------------------" << endl;

    Grafo g(6);
    g.adicionar_aresta(0, 1, 7, true);
    g.adicionar_aresta(0, 2, 9, true);
    g.adicionar_aresta(0, 5, 14, true);
    g.adicionar_aresta(1, 2, 10, true);
    g.adicionar_aresta(1, 3, 15, true);
    g.adicionar_aresta(2, 3, 11, true);
    g.adicionar_aresta(2, 5, 2, true);
    g.adicionar_aresta(3, 4, 6, true);
    g.adicionar_aresta(5, 4, 9, true);

    ResultadoDijkstra res_d2  = dijkstra_heap_dario(g, 0, 2);
    ResultadoDijkstra res_d4  = dijkstra_heap_dario(g, 0, 4);
    ResultadoDijkstra res_d8  = dijkstra_heap_dario(g, 0, 8);
    ResultadoDijkstra res_fib = dijkstra_heap_fibonacci(g, 0);

    validarResultadoQuatruplo(res_d2, res_d4, res_d8, res_fib, "Grafo Padrao");

    assert(res_d2.distancias[4] == 20);
    vector<int> caminho_esperado = {0, 2, 5, 4};
    assert(res_d2.reconstruir_caminho(4) == caminho_esperado);
    assert(res_d4.reconstruir_caminho(4) == caminho_esperado);
    assert(res_d8.reconstruir_caminho(4) == caminho_esperado);
    assert(res_fib.reconstruir_caminho(4) == caminho_esperado);

    cout << " [OK] Caminho minimo 0 -> 4 verificado: 0 -> 2 -> 5 -> 4 (Custo: 20)" << endl;
}

void testeGrafoNaoDirecionado() {
    cout << "\n--------------------------------------------------" << endl;
    cout << " [2] Teste Grafo Nao-Direcionado" << endl;
    cout << "--------------------------------------------------" << endl;

    Grafo g(4);
    g.adicionar_aresta(0, 1, 5, false);
    g.adicionar_aresta(1, 2, 3, false);
    g.adicionar_aresta(2, 3, 2, false);
    g.adicionar_aresta(0, 3, 12, false);

    ResultadoDijkstra res_d2  = dijkstra_heap_dario(g, 0, 2);
    ResultadoDijkstra res_d4  = dijkstra_heap_dario(g, 0, 4);
    ResultadoDijkstra res_d8  = dijkstra_heap_dario(g, 0, 8);
    ResultadoDijkstra res_fib = dijkstra_heap_fibonacci(g, 0);

    validarResultadoQuatruplo(res_d2, res_d4, res_d8, res_fib, "Grafo Nao-Direcionado");
    assert(res_d2.distancias[3] == 10);
    cout << " [OK] Arestas bidirecionais e atalhos calculados corretamente." << endl;
}

void testeVerticesInalcancaveis() {
    cout << "\n--------------------------------------------------" << endl;
    cout << " [3] Teste Vertices Inalcancaveis" << endl;
    cout << "--------------------------------------------------" << endl;

    Grafo g(5);
    g.adicionar_aresta(0, 1, 10, true);
    g.adicionar_aresta(1, 2, 5, true);

    ResultadoDijkstra res_d2  = dijkstra_heap_dario(g, 0, 2);
    ResultadoDijkstra res_d4  = dijkstra_heap_dario(g, 0, 4);
    ResultadoDijkstra res_d8  = dijkstra_heap_dario(g, 0, 8);
    ResultadoDijkstra res_fib = dijkstra_heap_fibonacci(g, 0);

    validarResultadoQuatruplo(res_d2, res_d4, res_d8, res_fib, "Vertices Inalcancaveis");
    assert(res_d2.distancias[3] == INT_MAX);
    assert(res_d2.reconstruir_caminho(3).empty());
    cout << " [OK] Vertices inalcancaveis mantiveram distancia INT_MAX e caminho vazio." << endl;
}

void testeCasosLimite() {
    cout << "\n--------------------------------------------------" << endl;
    cout << " [4] Teste Casos Limite (Vertice Unico e Self-Loops)" << endl;
    cout << "--------------------------------------------------" << endl;

    // V=1
    Grafo g1(1);
    ResultadoDijkstra res_g1_d2  = dijkstra_heap_dario(g1, 0, 2);
    ResultadoDijkstra res_g1_d4  = dijkstra_heap_dario(g1, 0, 4);
    ResultadoDijkstra res_g1_d8  = dijkstra_heap_dario(g1, 0, 8);
    ResultadoDijkstra res_g1_fib = dijkstra_heap_fibonacci(g1, 0);
    validarResultadoQuatruplo(res_g1_d2, res_g1_d4, res_g1_d8, res_g1_fib, "Caso V=1");

    // Self-loops
    Grafo g2(2);
    g2.adicionar_aresta(0, 0, 5, true);
    g2.adicionar_aresta(0, 1, 3, true);
    ResultadoDijkstra res_g2_d2  = dijkstra_heap_dario(g2, 0, 2);
    ResultadoDijkstra res_g2_d4  = dijkstra_heap_dario(g2, 0, 4);
    ResultadoDijkstra res_g2_d8  = dijkstra_heap_dario(g2, 0, 8);
    ResultadoDijkstra res_g2_fib = dijkstra_heap_fibonacci(g2, 0);
    validarResultadoQuatruplo(res_g2_d2, res_g2_d4, res_g2_d8, res_g2_fib, "Caso Self-Loops");

    cout << " [OK] Casos limite processados perfeitamente em todas as estruturas." << endl;
}

void testeExcecoes() {
    cout << "\n--------------------------------------------------" << endl;
    cout << " [5] Teste de Tratamento de Excecoes" << endl;
    cout << "--------------------------------------------------" << endl;

    int capturadas = 0;

    try {
        Grafo g(3);
        g.adicionar_aresta(0, 1, -5, true);
    } catch (const invalid_argument&) { capturadas++; }

    try {
        Grafo g(3);
        dijkstra_heap_dario(g, 10, 2);
    } catch (const out_of_range&) { capturadas++; }

    try {
        Grafo g(3);
        dijkstra_heap_dario(g, 10, 4);
    } catch (const out_of_range&) { capturadas++; }

    try {
        Grafo g(3);
        dijkstra_heap_dario(g, 10, 8);
    } catch (const out_of_range&) { capturadas++; }

    try {
        Grafo g(3);
        dijkstra_heap_fibonacci(g, 10);
    } catch (const out_of_range&) { capturadas++; }

    assert(capturadas == 5);
    cout << " [OK] Todas as excecoes obrigatorias foram lancadas e capturadas." << endl;
}

void testeConsistenciaEstresse(int num_vertices, int num_arestas) {
    cout << "\n--------------------------------------------------" << endl;
    cout << " [6] Teste de Estresse em Grafo Aleatorio (" << num_vertices << " V, " << num_arestas << " E)" << endl;
    cout << "--------------------------------------------------" << endl;

    Grafo g(num_vertices);
    mt19937 rng(42);
    uniform_int_distribution<int> dist_vertice(0, num_vertices - 1);
    uniform_int_distribution<int> dist_peso(1, 100);

    for (int i = 0; i < num_arestas; i++) {
        int u = dist_vertice(rng);
        int v = dist_vertice(rng);
        int peso = dist_peso(rng);
        g.adicionar_aresta(u, v, peso, true);
    }

    // 1. Heap Binário (d=2)
    auto t1 = chrono::steady_clock::now();
    ResultadoDijkstra res_d2 = dijkstra_heap_dario(g, 0, 2);
    auto t2 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_d2 = t2 - t1;

    // 2. 4-ary Heap (d=4)
    auto t3 = chrono::steady_clock::now();
    ResultadoDijkstra res_d4 = dijkstra_heap_dario(g, 0, 4);
    auto t4 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_d4 = t4 - t3;

    // 3. 8-ary Heap (d=8)
    auto t5 = chrono::steady_clock::now();
    ResultadoDijkstra res_d8 = dijkstra_heap_dario(g, 0, 8);
    auto t6 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_d8 = t6 - t5;

    // 4. Heap Fibonacci
    auto t7 = chrono::steady_clock::now();
    ResultadoDijkstra res_fib = dijkstra_heap_fibonacci(g, 0);
    auto t8 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_fib = t8 - t7;

    cout << " Tempo Heap Binario (d=2): " << fixed << setprecision(2) << tempo_d2.count() << " ms" << endl;
    cout << " Tempo 4-ary Heap  (d=4): " << tempo_d4.count() << " ms" << endl;
    cout << " Tempo 8-ary Heap  (d=8): " << tempo_d8.count() << " ms" << endl;
    cout << " Tempo Fibonacci Heap:    " << tempo_fib.count() << " ms" << endl;

    bool equivalente = validarResultadoQuatruplo(res_d2, res_d4, res_d8, res_fib, "Estresse Aleatorio");
    assert(equivalente);
}

// ------------------------------------------------------------------------------------------------------
// Execução Principal
// ------------------------------------------------------------------------------------------------------
int main() {
    cout << "==================================================" << endl;
    cout << "SUITE COMPLETA DE TESTES: d=2 vs d=4 vs d=8 vs FIB" << endl;
    cout << "==================================================" << endl;

    try {
        testeGrafoPadrao();
        testeGrafoNaoDirecionado();
        testeVerticesInalcancaveis();
        testeCasosLimite();
        testeExcecoes();
        testeConsistenciaEstresse(100000, 50000);

        cout << "\n==================================================" << endl;
        cout << "   [SUCESSO INTEGRAL] TODOS OS TESTES PASSARAM!   " << endl;
        cout << "==================================================" << endl;

    } catch (const exception& e) {
        cerr << "\n [FALHA DE TESTE] Excecao nao esperada: " << e.what() << endl;
        return 1;
    }

    return 0;
}