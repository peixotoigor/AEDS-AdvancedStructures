#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include "dijkstra.hpp"
#include "heap_dario.hpp"
#include "heap_fibonacci.hpp"
#include "leitor_dados.hpp" 
using namespace std;

void rodarBenchmarkArquivo(const string& caminho, const string& nome_cenario) {
    cout << "\n==================================================" << endl;
    cout << " BENCHMARK: " << nome_cenario << endl;
    cout << "==================================================" << endl;

    auto t_inicio = chrono::steady_clock::now();
    Grafo g = lerGrafoArquivo(caminho); //função do leitor_dados.hpp
    auto t_fim = chrono::steady_clock::now();
    chrono::duration<double> t_carga = t_fim - t_inicio;

    cout << "Grafo carregado em " << fixed << setprecision(2) << t_carga.count() << "s (" 
         << g.obter_num_vertices() << " V)" << endl << endl;

    int origem = 0;

    // 1. Heap Binario (d=2)
    auto t1 = chrono::steady_clock::now();
    ResultadoDijkstra res_d2 = dijkstra_heap_dario(g, origem, 2);
    auto t2 = chrono::steady_clock::now();

    // 2. 4-ary Heap (d=4)
    auto t3 = chrono::steady_clock::now();
    ResultadoDijkstra res_d4 = dijkstra_heap_dario(g, origem, 4);
    auto t4 = chrono::steady_clock::now();

    // 3. 8-ary Heap (d=8)
    auto t5 = chrono::steady_clock::now();
    ResultadoDijkstra res_d8 = dijkstra_heap_dario(g, origem, 8);
    auto t6 = chrono::steady_clock::now();

    // 4. Fibonacci Heap
    auto t7 = chrono::steady_clock::now();
    ResultadoDijkstra res_fib = dijkstra_heap_fibonacci(g, origem);
    auto t8 = chrono::steady_clock::now();

    chrono::duration<double, milli> tempo_d2  = t2 - t1;
    chrono::duration<double, milli> tempo_d4  = t4 - t3;
    chrono::duration<double, milli> tempo_d8  = t6 - t5;
    chrono::duration<double, milli> tempo_fib = t8 - t7;

    cout << " Tempo Heap Binario (d=2): " << setprecision(2) << tempo_d2.count() << " ms" << endl;
    cout << " Tempo 4-ary Heap  (d=4): " << tempo_d4.count() << " ms" << endl;
    cout << " Tempo 8-ary Heap  (d=8): " << tempo_d8.count() << " ms" << endl;
    cout << " Tempo Fibonacci Heap:    " << tempo_fib.count() << " ms" << endl;
}

int main() {
    try {
        rodarBenchmarkArquivo("data/grafo_esparso.dat", "GRAFO ESPARSO (E ~ 3V)");
        rodarBenchmarkArquivo("data/grafo_denso.dat", "GRAFO DENSO (E ~ 0.2 V^2)");
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }
    return 0;
}