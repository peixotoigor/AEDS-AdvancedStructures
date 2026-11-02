#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <filesystem>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__)
#include <sys/resource.h>
#endif

#include "dijkstra.hpp"
#include "leitor_dados.hpp"
#include "heap_dario.hpp"
#include "heap_fibonacci.hpp"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

const int REPETICOES = 10;
const int SEMENTE = 42;
const int VERTICE_ORIGEM = 0;

static size_t obter_memoria_kb() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024;
    }
    return 0;
#elif defined(__linux__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss;
    }
    return 0;
#else
    return 0;
#endif
}

static double calcular_media(const vector<double>& v) {
    if (v.empty()) return 0.0;
    double s = 0.0;
    for (double x : v) s += x;
    return s / v.size();
}

static double calcular_desvio_padrao(const vector<double>& v, double media) {
    if (v.size() < 2) return 0.0;
    double s = 0.0;
    for (double x : v) s += (x - media) * (x - media);
    return std::sqrt(s / (v.size() - 1));
}

volatile int64_t g_acumulador_dijkstra = 0;

int main() {
    cout << "========================================================================\n";
    cout << "  ESTUDO 2 (CENARIO 2): DIJKSTRA COM HEAP FIBONACCI VS HEAPS D-ARIOS    \n";
    cout << "  Grafos Esparsos e Densos | seed = " << SEMENTE << " | C++20 (-O3)                \n";
    cout << "========================================================================\n\n";

    fs::create_directories("results");
    string caminho_csv = "results/estudo2_cenario2_dijkstra.csv";
    ofstream csv(caminho_csv);
    csv << "tipo_grafo,num_vertices,num_arestas,fila_prioridade,d,tempo_medio_ms,desvio_padrao_ms,memoria_kb\n";

    vector<pair<string, string>> arquivos_grafos = {
        {"esparso", "data/grafo_esparso.dat"},
        {"denso", "data/grafo_denso.dat"}
    };

    struct VarianteHeap {
        string nome;
        int d; // 0 para fibonacci, d para d-ario
    };

    vector<VarianteHeap> variantes = {
        {"Heap Binario (d=2)", 2},
        {"Heap 4-ario (d=4)", 4},
        {"Heap 8-ario (d=8)", 8},
        {"Heap de Fibonacci", 0}
    };

    for (const auto& [tipo_grafo, caminho_arquivo] : arquivos_grafos) {
        cout << "\n>>> CARREGANDO GRAFO: " << tipo_grafo << " (" << caminho_arquivo << ")..." << endl;
        Grafo grafo = lerGrafoArquivo(caminho_arquivo);
        int V = grafo.obter_num_vertices();
        int E = grafo.obter_num_arestas();
        cout << "    Vertices (V): " << V << " | Arestas (E): " << E << endl;
        cout << "------------------------------------------------------------------------\n";

        for (const auto& var : variantes) {
            vector<double> tempos_ms;
            size_t mem_pico = 0;

            // Warm-up preliminar descartado
            {
                ResultadoDijkstra r_w;
                if (var.d == 0) {
                    r_w = dijkstra_heap_fibonacci(grafo, VERTICE_ORIGEM);
                } else {
                    r_w = dijkstra_heap_dario(grafo, VERTICE_ORIGEM, var.d);
                }
                g_acumulador_dijkstra += r_w.distancias[V - 1];
            }

            // 10 Repeticoes Independentes
            for (int rep = 0; rep < REPETICOES; rep++) {
                auto t0 = high_resolution_clock::now();
                ResultadoDijkstra res;

                if (var.d == 0) {
                    res = dijkstra_heap_fibonacci(grafo, VERTICE_ORIGEM);
                } else {
                    res = dijkstra_heap_dario(grafo, VERTICE_ORIGEM, var.d);
                }

                auto t1 = high_resolution_clock::now();
                tempos_ms.push_back(duration<double, milli>(t1 - t0).count());
                g_acumulador_dijkstra += res.distancias[V / 2];

                if (rep == 0) mem_pico = obter_memoria_kb();
            }

            double media_ms = calcular_media(tempos_ms);
            double desvio_ms = calcular_desvio_padrao(tempos_ms, media_ms);

            csv << tipo_grafo << "," << V << "," << E << ","
                << var.nome << "," << var.d << ","
                << media_ms << "," << desvio_ms << "," << mem_pico << "\n";

            cout << "  [" << setw(20) << left << var.nome << "] Tempo medio: "
                 << setw(10) << right << fixed << setprecision(2) << media_ms << " ms | "
                 << "Desvio: " << setw(8) << desvio_ms << " ms | "
                 << "Memoria: " << setw(8) << mem_pico << " KB\n";
        }
    }

    csv.close();
    cout << "\n========================================================================\n";
    cout << " CENARIO 2 CONCLUIDO COM SUCESSO!\n";
    cout << " Resultados gravados em: " << caminho_csv << "\n";
    cout << " Acumulador de integridade: " << g_acumulador_dijkstra << "\n";
    cout << "========================================================================\n";

    return 0;
}

