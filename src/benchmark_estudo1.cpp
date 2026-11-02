#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <numeric>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__)
#include <sys/resource.h>
#endif

#include "heap_fibonacci.hpp"
#include "veb.hpp"

using namespace std;
using namespace std::chrono;

namespace fs = std::filesystem;

// Constantes experimentais definidas pelo protocolo
const int REPETICOES = 10;
const int UNIVERSO_U = 1048576; // 2^20 (comporta valores ate 1.000.000)
const vector<int> TAMANHOS_N = {100, 1000, 10000, 100000, 1000000};
const vector<pair<string, string>> PADROES_ENTRADA = {
    {"cresce", "data/crescente.dat"},
    {"decresce", "data/decrescente.dat"},
    {"aleatorio", "data/aleatorio.dat"}
};

// Medição de consumo de memória em KB
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

// Estrutura para consolidação das métricas experimentais
struct RegistroResultado {
    string estrutura;
    string padrao_entrada;
    int n;
    int universo_u;
    double razao_ocupacao_rho;
    string operacao;
    double tempo_medio_us;
    double desvio_padrao_us;
    double tempo_por_op_ns;
    size_t memoria_kb;
};

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

// Carregador de dados com prefixo n
static vector<int> carregar_prefixo(const string& caminho, int n) {
    vector<int> dados;
    dados.reserve(n);
    ifstream arq(caminho);
    if (!arq.is_open()) {
        cerr << "[ERRO] Nao foi possivel abrir o arquivo: " << caminho << endl;
        exit(1);
    }
    int val;
    while (dados.size() < static_cast<size_t>(n) && arq >> val) {
        dados.push_back(val);
    }
    return dados;
}

// Variável auxiliar volátil para evitar Dead Code Elimination (DCE) no -O3
volatile uint64_t g_acumulador_auxiliar = 0;

int main() {
    cout << "========================================================================\n";
    cout << "  ESTUDO 1: COMPARACAO ISOLADA DAS FILAS DE PRIORIDADE                 \n";
    cout << "  Heap de Fibonacci vs Arvore de van Emde Boas (vEB)                   \n";
    cout << "  Protocolo: 10 repeticoes | Aquecimento preliminar | C++20 (-O3)     \n";
    cout << "========================================================================\n\n";

    fs::create_directories("results");
    string caminho_csv = "results/estudo1_filas_prioridade.csv";
    ofstream csv(caminho_csv);
    csv << "estrutura,padrao_entrada,n,universo_u,razao_ocupacao_rho,operacao,"
        << "tempo_medio_us,desvio_padrao_us,tempo_por_op_ns,memoria_kb\n";

    vector<RegistroResultado> todos_resultados;

    for (const auto& [padrao_nome, padrao_arquivo] : PADROES_ENTRADA) {
        cout << "\n>>> PADRAO DE ENTRADA: " << padrao_nome << " (" << padrao_arquivo << ")\n";
        cout << "------------------------------------------------------------------------\n";

        for (int n : TAMANHOS_N) {
            double rho = static_cast<double>(n) / UNIVERSO_U;
            cout << "\n[N = " << setw(7) << n << " | U = " << UNIVERSO_U << " | rho = " << fixed << setprecision(6) << rho << "]\n";

            vector<int> dados = carregar_prefixo(padrao_arquivo, n);

            // =================================================================
            // 1. AVALIACAO DO HEAP DE FIBONACCI
            // =================================================================
            {
                vector<double> tempos_insert;
                vector<double> tempos_findmin;
                vector<double> tempos_extractmin;
                vector<double> tempos_delete;
                size_t mem_pico = 0;

                // Rodada de Aquecimento (Warm-up descartada das estatísticas)
                {
                    HeapFibonacci H_warm;
                    vector<HeapFibonacci::NoFibonacci*> nos_warm(n);
                    for (int i = 0; i < n; i++) {
                        nos_warm[i] = H_warm.insert(dados[i], dados[i]);
                    }
                    g_acumulador_auxiliar += H_warm.minimum()->chave;
                    for (int i = 0; i < n; i++) {
                        auto no = H_warm.delete_no(nos_warm[i]);
                        delete no;
                    }
                }

                // 10 Repetições Independentes
                for (int rep = 0; rep < REPETICOES; rep++) {
                    // a) Medir Inserir elemento (insert(x))
                    auto H = new HeapFibonacci();
                    vector<HeapFibonacci::NoFibonacci*> nos(n);

                    auto t0 = high_resolution_clock::now();
                    for (int i = 0; i < n; i++) {
                        nos[i] = H->insert(dados[i], dados[i]);
                    }
                    auto t1 = high_resolution_clock::now();
                    tempos_insert.push_back(duration<double, micro>(t1 - t0).count());
                    if (rep == 0) mem_pico = obter_memoria_kb();

                    // b) Medir Obter o mínimo (findMin())
                    // Consulta executada em laço com acumulador auxiliar
                    int consultas = std::max(n, 10000);
                    auto t2 = high_resolution_clock::now();
                    for (int c = 0; c < consultas; c++) {
                        g_acumulador_auxiliar += H->minimum()->chave;
                    }
                    auto t3 = high_resolution_clock::now();
                    double tempo_findmin_total = duration<double, micro>(t3 - t2).count();
                    tempos_findmin.push_back(tempo_findmin_total * (static_cast<double>(n) / consultas));

                    // c) Medir Remover elemento específico (delete(x)) usando referências estruturais
                    auto t4 = high_resolution_clock::now();
                    for (int i = 0; i < n; i++) {
                        auto no_del = H->delete_no(nos[i]);
                        delete no_del;
                    }
                    auto t5 = high_resolution_clock::now();
                    tempos_delete.push_back(duration<double, micro>(t5 - t4).count());

                    // Verificação auxiliar de integridade (isEmpty())
                    if (!H->esta_vazio()) {
                        cerr << "[ALERTA] Heap de Fibonacci nao esvaziou corretamente em delete(x)!\n";
                    }
                    delete H;

                    // d) Medir Remover o mínimo (extractMin()) reconstruindo a estrutura
                    auto H_ext = new HeapFibonacci();
                    for (int i = 0; i < n; i++) {
                        H_ext->insert(dados[i], dados[i]);
                    }

                    auto t6 = high_resolution_clock::now();
                    for (int i = 0; i < n; i++) {
                        auto no_min = H_ext->extract_min();
                        g_acumulador_auxiliar += no_min->chave;
                        delete no_min;
                    }
                    auto t7 = high_resolution_clock::now();
                    tempos_extractmin.push_back(duration<double, micro>(t7 - t6).count());

                    if (!H_ext->esta_vazio()) {
                        cerr << "[ALERTA] Heap de Fibonacci nao esvaziou corretamente em extractMin()!\n";
                    }
                    delete H_ext;
                }

                auto registrar = [&](const string& op, const vector<double>& tempos) {
                    double media = calcular_media(tempos);
                    double desvio = calcular_desvio_padrao(tempos, media);
                    double ns_op = (media * 1000.0) / n;
                    todos_resultados.push_back({"HeapFibonacci", padrao_nome, n, UNIVERSO_U, rho, op, media, desvio, ns_op, mem_pico});
                    csv << "HeapFibonacci," << padrao_nome << "," << n << "," << UNIVERSO_U << "," << rho << ","
                        << op << "," << media << "," << desvio << "," << ns_op << "," << mem_pico << "\n";
                };

                registrar("insert(x)", tempos_insert);
                registrar("findMin()", tempos_findmin);
                registrar("extractMin()", tempos_extractmin);
                registrar("delete(x)", tempos_delete);

                cout << "  [HeapFibonacci] insert: " << setw(10) << fixed << setprecision(2) << calcular_media(tempos_insert) << " us | "
                     << "extractMin: " << setw(10) << calcular_media(tempos_extractmin) << " us | "
                     << "delete(x): " << setw(10) << calcular_media(tempos_delete) << " us\n";
            }

            // =================================================================
            // 2. AVALIACAO DA ARVORE VAN EMDE BOAS (vEB)
            // =================================================================
            {
                vector<double> tempos_insert;
                vector<double> tempos_findmin;
                vector<double> tempos_extractmin;
                vector<double> tempos_delete;
                size_t mem_pico = 0;

                // Rodada de Aquecimento (Warm-up descartada das estatísticas)
                {
                    vanEmdeBoas* V_warm = new vanEmdeBoas(UNIVERSO_U);
                    for (int i = 0; i < n; i++) {
                        veb_insert(V_warm, dados[i]);
                    }
                    g_acumulador_auxiliar += veb_minimum(V_warm);
                    for (int i = 0; i < n; i++) {
                        veb_delete(V_warm, dados[i]);
                    }
                    delete V_warm;
                }

                // 10 Repetições Independentes
                for (int rep = 0; rep < REPETICOES; rep++) {
                    // a) Medir Inserir elemento (insert(x))
                    vanEmdeBoas* V = new vanEmdeBoas(UNIVERSO_U);

                    auto t0 = high_resolution_clock::now();
                    for (int i = 0; i < n; i++) {
                        veb_insert(V, dados[i]);
                    }
                    auto t1 = high_resolution_clock::now();
                    tempos_insert.push_back(duration<double, micro>(t1 - t0).count());
                    if (rep == 0) mem_pico = obter_memoria_kb();

                    // b) Medir Obter o mínimo (minimum())
                    int consultas = std::max(n, 10000);
                    auto t2 = high_resolution_clock::now();
                    for (int c = 0; c < consultas; c++) {
                        g_acumulador_auxiliar += veb_minimum(V);
                    }
                    auto t3 = high_resolution_clock::now();
                    double tempo_findmin_total = duration<double, micro>(t3 - t2).count();
                    tempos_findmin.push_back(tempo_findmin_total * (static_cast<double>(n) / consultas));

                    // c) Medir Remover elemento específico (delete(x)) por chave
                    auto t4 = high_resolution_clock::now();
                    for (int i = 0; i < n; i++) {
                        veb_delete(V, dados[i]);
                    }
                    auto t5 = high_resolution_clock::now();
                    tempos_delete.push_back(duration<double, micro>(t5 - t4).count());

                    // Verificação auxiliar de integridade (isEmpty())
                    if (veb_minimum(V) != vEB_NIL) {
                        cerr << "[ALERTA] Arvore vEB nao esvaziou corretamente em delete(x)!\n";
                    }
                    delete V;

                    // d) Medir Remover o mínimo (delete(minimum())) reconstruindo a estrutura
                    vanEmdeBoas* V_ext = new vanEmdeBoas(UNIVERSO_U);
                    for (int i = 0; i < n; i++) {
                        veb_insert(V_ext, dados[i]);
                    }

                    auto t6 = high_resolution_clock::now();
                    for (int i = 0; i < n; i++) {
                        int m = veb_minimum(V_ext);
                        g_acumulador_auxiliar += m;
                        veb_delete(V_ext, m);
                    }
                    auto t7 = high_resolution_clock::now();
                    tempos_extractmin.push_back(duration<double, micro>(t7 - t6).count());

                    if (veb_minimum(V_ext) != vEB_NIL) {
                        cerr << "[ALERTA] Arvore vEB nao esvaziou corretamente em delete(minimum())!\n";
                    }
                    delete V_ext;
                }

                auto registrar = [&](const string& op, const vector<double>& tempos) {
                    double media = calcular_media(tempos);
                    double desvio = calcular_desvio_padrao(tempos, media);
                    double ns_op = (media * 1000.0) / n;
                    todos_resultados.push_back({"vanEmdeBoas", padrao_nome, n, UNIVERSO_U, rho, op, media, desvio, ns_op, mem_pico});
                    csv << "vanEmdeBoas," << padrao_nome << "," << n << "," << UNIVERSO_U << "," << rho << ","
                        << op << "," << media << "," << desvio << "," << ns_op << "," << mem_pico << "\n";
                };

                registrar("insert(x)", tempos_insert);
                registrar("minimum()", tempos_findmin);
                registrar("delete(minimum())", tempos_extractmin);
                registrar("delete(x)", tempos_delete);

                cout << "  [vanEmdeBoas ] insert: " << setw(10) << fixed << setprecision(2) << calcular_media(tempos_insert) << " us | "
                     << "extractMin: " << setw(10) << calcular_media(tempos_extractmin) << " us | "
                     << "delete(x): " << setw(10) << calcular_media(tempos_delete) << " us\n";
            }
        }
    }

    csv.close();
    cout << "\n========================================================================\n";
    cout << " EXPERIMENTO CONCLUIDO COM SUCESSO!\n";
    cout << " Dados salvos em: " << caminho_csv << "\n";
    cout << " Acumulador de integridade de consultas: " << g_acumulador_auxiliar << "\n";
    cout << "========================================================================\n";

    return 0;
}

